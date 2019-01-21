#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/multi_index.hpp>

using namespace eosio;
using namespace std;

CONTRACT eoseosrateio : public contract {
  public:
    using contract::contract;

    ACTION rateproducer(name user, name bp, string ratings_json) {
      require_auth(user);

      // user must be a proxy
      voters_table _voters(_self, "eosio"_n.value);
      auto pitr = _voters.find( user.value );
      if ( pitr != _voters.end() ) {
        eosio_assert( !pitr->is_proxy, "only proxy accounts are allowed to rate at the moment" );
      }

      //TODO: bp must be a registered block producer

      // the payload must be ratings_json.
      eosio_assert(ratings_json[0] == '{', "payload must be ratings_json");
      eosio_assert(ratings_json[ratings_json.size()-1] == '}', "payload must be ratings_json");

      // upsert bp rating
      rate_table _ratings(_self, _self.value);
      auto uniq_rating = (static_cast<uint128_t>(user.value) << 64) | bp.value;
      auto uniq_rating_index = _ratings.get_index<name("uniqrating")>();
      auto existing_rating = uniq_rating_index.find(uniq_rating);

      if( existing_rating == _ratings.end() ) {
        _ratings.emplace(user, [&]( auto& row ) {
          row.id = _ratings.available_primary_key();
          row.uniq_rating = uniq_rating;
          row.user = user;
          row.bp = bp;
          row.created_at = current_time();
          row.ratings_json = ratings_json;
        });
      } else {
        _ratings.modify(existing_rating, user, [&]( auto& row ) {
          row.user = user;
          row.bp = bp;
          row.created_at = current_time();
          row.ratings_json = ratings_json;
        });
      }
    }

  private:
    TABLE rating {
      uint64_t id;
      uint128_t uniq_rating;
      name user;
      name bp;
      string ratings_json;
      uint64_t created_at;

      uint64_t primary_key() const { return id; }
      uint128_t by_uniq_rating() const { return uniq_rating; }
      uint64_t by_user() const { return user.value; }
      uint64_t by_bp() const { return bp.value; }
    };

    typedef eosio::multi_index<"ratings"_n, rating,
        indexed_by<"uniqrating"_n, const_mem_fun<rating, uint128_t, &rating::by_uniq_rating>>,
        indexed_by<"user"_n, const_mem_fun<rating, uint64_t, &rating::by_user>>,
        indexed_by<"bp"_n, const_mem_fun<rating, uint64_t, &rating::by_bp>>
      > rate_table;

    TABLE voter_info {
      name                owner;     /// the voter
      name                proxy;     /// the proxy set by the voter, if any
      std::vector<name>   producers; /// the producers approved by this voter if no proxy set
      int64_t             staked = 0;
      double              last_vote_weight = 0; /// the vote weight cast the last time the vote was updated
      double              proxied_vote_weight= 0; /// the total vote weight delegated to this voter as a proxy
      bool                is_proxy = 0; /// whether the voter is a proxy for others
      uint32_t            reserved1 = 0;
      uint32_t            reserved2 = 0;
      eosio::asset        reserved3;

      uint64_t primary_key()const { return owner.value; }

      // explicit serialization macro is not necessary, used here only to improve compilation time
      EOSLIB_SERIALIZE( voter_info, (owner)(proxy)(producers)(staked)(last_vote_weight)(proxied_vote_weight)(is_proxy)(reserved1)(reserved2)(reserved3) )
    };

    typedef eosio::multi_index<"voters"_n, eoseosrateio::voter_info > voters_table;
};

EOSIO_DISPATCH(eoseosrateio, (rateproducer));
