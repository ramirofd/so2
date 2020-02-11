document.addEventListener('DOMContentLoaded', function() {
    var memoryChartOpt = {
        chart: {
            height: 500,
            type: 'radialBar',
        },
        plotOptions: {
            radialBar: {
                startAngle: -135,
                endAngle: 135,
                dataLabels: {
                    name: {
                        fontSize: '16px',
                        color: undefined,
                        offsetY: 120
                    },
                    value: {
                        offsetY: 76,
                        fontSize: '22px',
                        color: undefined,
                        formatter: function (val) {
                            return val + "%";
                        }
                    }
                }
            }
        },
        fill: {
            type: 'gradient',
            gradient: {
                shade: 'dark',
                shadeIntensity: 0.15,
                inverseColors: false,
                opacityFrom: 1,
                opacityTo: 1,
                stops: [0, 50, 65, 91]
            },
        },
        stroke: {
            dashArray: 2
        },
        series: [0],
        labels: ['Used Memory'],
    };
    
    var cpuChartOpt = {
        chart: {
            height: 500,
            type: 'radialBar',
        },

        plotOptions: {
            radialBar: {
                startAngle: -135,
                endAngle: 135,
                dataLabels: {
                    name: {
                        fontSize: '16px',
                        color: undefined,
                        offsetY: 120
                    },
                    value: {
                        offsetY: 76,
                        fontSize: '22px',
                        color: undefined,
                        formatter: function (val) {
                            return val + "%";
                        }
                    }
                }
            }
        },
        fill: {
            type: 'gradient',
            gradient: {
                shade: 'dark',
                shadeIntensity: 0.15,
                inverseColors: false,
                opacityFrom: 1,
                opacityTo: 1,
                stops: [0, 50, 65, 91]
            },
        },
        stroke: {
            dashArray: 2
        },
        series: [0],
        labels: ['Used CPU'],
    };
    var xhr = null;

    getXmlHttpRequestObject = function()
    {
        if(!xhr)
        {               
            // Create a new XMLHttpRequest object 
            xhr = new XMLHttpRequest();
        }
        return xhr;
    };

    updateLiveData = function()
    {
        var now = new Date();
        // Date string is appended as a query with live data 
        // for not to use the cached version 
        var url = 'http://192.168.43.235/cgi-bin/sysinfo.cgi';
        xhr = getXmlHttpRequestObject();
        xhr.onreadystatechange = evenHandler;
        // asynchronous requests
        xhr.open("GET", url, true);
        // Send the request over the network
        xhr.send(null);
    };

    updateLiveData();

    function evenHandler()
    {
        // Check response is ready or not
        if(xhr.readyState == 4 && xhr.status == 200)
        {
            uptime = document.getElementById('uptimeH');
	    datetime = document.getElementById('datetimeH');
            // Set current data text
            var arrResponse = JSON.parse(xhr.responseText);
	    
    	    cpuChart.updateSeries([Math.floor(arrResponse['data']['cpu'])]);

    	    memoryChart.updateSeries([Math.floor(arrResponse['data']['memory'])]);
            // Update the live data every 1 sec
	    uptime.innerHTML = arrResponse['data']['uptime'];
	    datetime.innerHTML = arrResponse['data']['datetime'];
            setTimeout(updateLiveData(), 1000);
	    
        }
    }
    var memoryChart = new ApexCharts(document.querySelector("#memoryChart"), memoryChartOpt);
    memoryChart.render();
    
    var cpuChart = new ApexCharts(document.querySelector("#cpuChart"), cpuChartOpt);
    cpuChart.render();
    
 }, false);
 
function updateCpuChart(){
    cpuChart.updateSeries([Math.floor(Math.random() * 101)], true)
}

function updateMemoryChart(){
    memoryChart.updateSeries([Math.floor(Math.random() * 101)], true)
}
