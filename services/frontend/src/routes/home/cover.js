import React from 'react'
import { useTranslation } from 'react-i18next'
import Typography from '@material-ui/core/Typography'
import Button from '@material-ui/core/Button'
import Grid from '@material-ui/core/Grid'
import { withStyles } from '@material-ui/core/styles'
import { Link } from '@reach/router'
import PropTypes from 'prop-types'

import Radar from 'components/radar'

const bpLink = React.forwardRef((props, ref) => (
  <Link innerRef={ref} {...props} />
))
const styles = ({ palette, typography }) => ({
  coverContainer: {
    padding: 0,
    color: '#433F5B',
    maxWidth: '1024px',
    backgroundColor: palette.surface.main
  },
  title: {
    fontSize: typography.h4.fontSize,
    marginBottom: 12.5
  },
  paragraph: {
    //    color: palette.grey[600]
  },
  ctaContainer: {
    textAlign: 'center'
  },
  chartContainer: {
    maxWidth: '400px',
    width: '100%',
    display: 'flex',
    alignItems: 'center'
  },
  btn: {},
  leftCoverBox: {
    display: 'flex',
    flexDirection: 'column',
    justifyContent: 'space-between'
  }
})

const HomeCover = ({ classes, blockProducer }) => {
  const { t } = useTranslation('home')

  return (
    <Grid item container xs={12} className={classes.coverContainer}>
      <Grid item xs={12} md={6} className={classes.leftCoverBox}>
        <Typography variant='h5' className={classes.title}>
          {t('cover.title')}
        </Typography>
        <Typography
          variant='body2'
          className={classes.paragraph}
          align='justify'
          paragraph
        >
          {t('cover.paragraph1')}
        </Typography>
        <Typography
          variant='body2'
          className={classes.paragraph}
          align='justify'
          paragraph
        >
          {t('cover.paragraph2')}
        </Typography>
        <div className={classes.ctaContainer}>
          <Button
            className={classes.btn}
            component={bpLink}
            variant='contained'
            size='medium'
            color='secondary'
            to='/block-producers'
            fullWidth
          >
            {t('cover.cta')}
          </Button>
        </div>
      </Grid>

      <Grid item container xs={12} md={6} justify='center'>
        <div className={classes.chartContainer}>
          <Radar
            height={230}
            bpData={{
              datasets: [blockProducer.data]
            }}
          />
        </div>
      </Grid>

      {/* <Grid item xs={12}>
      <ParameterRangeSelector defaultValue={[0, 50]} />
    </Grid> */}
    </Grid>
  )
}

HomeCover.propTypes = {
  classes: PropTypes.object.isRequired,
  blockProducer: PropTypes.object.isRequired
}

export default withStyles(styles)(HomeCover)
