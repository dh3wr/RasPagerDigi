var chartVoltage;
var chartAmpere;
var chartPower;


var vm = new Vue({
    el: "#wrapper",
    created() {
        this.connect();
    },
    data: {
        connected: false,
        Voltage: 0.0,
		Current: 0.0,
		PowerForward: 0.0,
		PowerForwardLastTX: 0.0,
		PowerReflect: 0.0,
		PowerReflectLastTX: 0.0,
		PowerVSWR: 0.0,
		PowerVSWRLastTX: 0.0,
		Slots: ""
    },
    methods: {
        connect: function(event) {
            this.socket = new WebSocket("ws://" + "44.225.164.233" + ":8080/echo_all");
            this.socket.onopen = this.onopen;
            this.socket.onmessage = this.onmessage;
            this.socket.onclose = this.onclose;
        },
        onopen: function(event) {
            this.connected = true;
            console.log("Connected to RasPagerDigi");
//            this.send("GetVersion");
//            this.send("GetConfig");
        },
        onmessage: function(event) {
            var response = JSON.parse(event.data) || {};
			console.log(response);
            for (var key in response) {
                var value = response[key];
                switch (key) {
                    case "Voltage": this.Voltage = value; break;
                    case "Current": this.Current = value; break;
                    case "PowerForward": this.PowerForward = value; break;
                    case "PowerForwardLastTX": this.PowerForwardLastTX = value; break;
                    case "PowerReflect": this.PowerReflect = value; break;
                    case "PowerReflectLastTX": this.PowerReflectLastTX = value; break;
                    case "PowerVSWR": this.PowerVSWR = value; break;
                    case "PowerVSWRLastTX": this.PowerVSWRLastTX = value; break;
                    case "Slots": this.Slots = value; break;
                    default: console.log("Unknown Key: ", key);
                }
            }
			        // Voltage
			var point;

			if (chartVoltage) {
				point = chartVoltage.series[0].points[0];

				point.update(this.Voltage);
				console.log(this.Voltage);
			}

			// Ampere
			if (chartAmpere) {
				point = chartAmpere.series[0].points[0];

				point.update(this.Current);
			}
			
			// Power
			if (chartPower) { // the chart may be destroyed
                var left = chartPower.series[0].points[0],
					right = chartPower.series[1].points[0];

				left.update(this.PowerForward, false);
				right.update(this.PowerReflect, false);
				chartPower.redraw();
			}
        },
        onclose: function(event) {
            if (this.connected) {
				console.log("Disconnected from RasPagerDigi");
            }
            this.connected = false;
            setTimeout(function() { this.connect(); }.bind(this), 1000);
        },
        send: function(data) {
            this.socket.send(JSON.stringify(data));
        },
/*
        restart: function(event) {
            this.send("Restart");
        },
        
        send_message: function(event)  {
            localStorage && (localStorage.pager_addr = this.addr);

            var req = {"SendMessage": {
                "addr": parseInt(this.addr) || 0,
                "data": this.message
            }};

            this.send(req);
            this.message = "";
        }
*/
    }
});


$(function () {

    var gaugeOptions = {

        chart: {
            type: 'solidgauge'
        },

        title: null,

        pane: {
            center: ['50%', '85%'],
            size: '140%',
            startAngle: -90,
            endAngle: 90,
            background: {
                backgroundColor: (Highcharts.theme && Highcharts.theme.background2) || '#EEE',
                innerRadius: '60%',
                outerRadius: '100%',
                shape: 'arc'
            }
        },

        tooltip: {
            enabled: false
        },

        // the value axis
        yAxis: {
            stops: [
                [0.1, '#55BF3B'], // green
                [0.5, '#DDDF0D'], // yellow
                [0.9, '#DF5353'] // red
            ],
            lineWidth: 0,
            minorTickInterval: null,
            tickAmount: 2,
            title: {
                y: -70
            },
            labels: {
                y: 16
            }
        },

        plotOptions: {
            solidgauge: {
                dataLabels: {
                    y: 5,
                    borderWidth: 0,
                    useHTML: true
                }
            }
        }
    };

    // The voltage gauge
    chartVoltage = Highcharts.chart('container-voltage', Highcharts.merge(gaugeOptions, {
        yAxis: {
            min: 0,
            max: 16,
            title: {
                text: 'Voltage'
            }
        },

        credits: {
            enabled: false
        },

        series: [{
            name: 'Voltage',
            data: [80],
            dataLabels: {
                format: '<div style="text-align:center"><span style="font-size:25px;color:' +
                    ((Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black') + '">{y}</span><br/>' +
                       '<span style="font-size:12px;color:silver">V</span></div>'
            },
            tooltip: {
                valueSuffix: ' V'
            }
        }]

    }));

    // The Ampere gauge
    chartAmpere = Highcharts.chart('container-ampere', Highcharts.merge(gaugeOptions, {
        yAxis: {
            min: 0,
            max: 10,
            title: {
                text: 'Ampere'
            }
        },

        series: [{
            name: 'Ampere',
            data: [1],
            dataLabels: {
                format: '<div style="text-align:center"><span style="font-size:25px;color:' +
                    ((Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black') + '">{y:.1f}</span><br/>' +
                       '<span style="font-size:12px;color:silver">A</span></div>'
            },
            tooltip: {
                valueSuffix: ' A'
            }
        }]

    }));
	
	chartPower = Highcharts.chart('container-power', {

        chart: {
            type: 'gauge',
            plotBorderWidth: 1,
            plotBackgroundColor: {
                linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                stops: [
                    [0, '#FFF4C6'],
                    [0.3, '#FFFFFF'],
                    [1, '#FFF4C6']
                ]
            },
            plotBackgroundImage: null,
            height: 200
        },

        title: {
            text: 'Power'
        },

        pane: [{
            startAngle: -45,
            endAngle: 45,
            background: null,
            center: ['25%', '145%'],
            size: 300
        }, {
            startAngle: -45,
            endAngle: 45,
            background: null,
            center: ['75%', '145%'],
            size: 300
        }],

        tooltip: {
            enabled: false
        },

        yAxis: [{
            min: 0,
            max: 40,
            minorTickPosition: 'outside',
            tickPosition: 'outside',
            labels: {
                rotation: 'auto',
                distance: 20
            },
            plotBands: [{
                from: 0,
                to: 30,
                color: '#00FF00',
                innerRadius: '100%',
                outerRadius: '105%'
            }, {
                from: 30,
                to: 35,
                color: '#FFFF00',
                innerRadius: '100%',
                outerRadius: '105%'
			}, {
                from: 35,
                to: 40,
                color: '#C02316',
                innerRadius: '100%',
                outerRadius: '105%'
			}],
            pane: 0,
            title: {
                text: 'Forward Power<br/><span style="font-size:10px">Watt</span>',
                y: -40
            }
        }, {
            min: 0,
            max: 40,
            minorTickPosition: 'outside',
            tickPosition: 'outside',
            labels: {
                rotation: 'auto',
                distance: 20
            },
            plotBands: [{
                from: 0,
                to: 3,
                color: '#00FF00',
                innerRadius: '100%',
                outerRadius: '105%'
            }, {
                from: 3,
                to: 7,
                color: '#FFFF00',
                innerRadius: '100%',
                outerRadius: '105%'
			}, {
                from: 7,
                to: 40,
                color: '#C02316',
                innerRadius: '100%',
                outerRadius: '105%'
			}],
            pane: 1,
            title: {
                text: 'Reflected Power<br/><span style="font-size:10px">Watt</span>',
                y: -40
            }
        }],

        plotOptions: {
            gauge: {
                dataLabels: {
                    enabled: true,
					align: 'center',
					y: -40,
					borderWidth: 0,
	                format: '<div style="text-align:center"><span style="font-size:25px;color:' +
						((Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black') + '">{y:.2f}</span></div>',
                },
                dial: {
                    radius: '100%'
                }
            }
        },


        series: [{
            name: 'Channel A',
            data: [0],
            yAxis: 0
        }, {
            name: 'Channel B',
            data: [0],
            yAxis: 1
        }]

    });


});
