
<?php 

echo $variables['bugs'];

?>

<script type="text/javascript">
<?php
$data = $variables['bugs'];

?>
(function ($) {
  $(document).ready (function() {
   var chart;
        chart = new Highcharts.Chart({
            chart: {
                renderTo: 'container',
                plotBackgroundColor: null,
                plotBorderWidth: null,
                plotShadow: false
            },
            title: {
                text: 'Heineken Bug Ranking'
            },
            tooltip: {
                pointFormat: '{series.name}: <b>{point.percentage}%</b>',
                percentageDecimals: 1
            },
            plotOptions: {
                pie: {
                    allowPointSelect: true,
                    cursor: 'pointer',
                    dataLabels: {
                        enabled: true,
                        color: '#000000',
                        connectorColor: '#000000',
                        formatter: function() {
                            return '<b>'+ this.point.name +'</b>: '+ this.percentage +' %';
                        }
                    }
                }
            },
            series: [{
                type: 'pie',
                name: 'Heineken Bug Ranking',
                <?php echo $data ?>
            }]
        }); 
  });
})(jQuery);
</script>
<div id="container" style="min-width: 400px; height: 400px; margin: 0 auto"></div>

