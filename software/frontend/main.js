var chartVoltage;
var chartAmpere;
var chartPower;
var chartIntTemp;
var chartExtTemp;


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
		Slots: "",
		INTemp: "",
		OUTTemp: "",
		APRSTemp: "",
		PATemp: "",
		ExtTemp1: "",
		ExtTemp2: "",
		ExtTemp3: "",
		ExtTemp4: ""
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
//		console.log(response);
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
					case "INTemp" : this.INTemp = value; break;
					case "OUTTemp" : this.OUTTemp = value; break;
					case "APRSTemp" : this.APRSTemp = value; break;
					case "PATemp" : this.PATemp = value; break;
					case "ExtTemp1" : this.ExtTemp1 = value; break;
					case "ExtTemp2" : this.ExtTemp2 = value; break;
					case "ExtTemp3" : this.ExtTemp3 = value; break;
					case "ExtTemp4" : this.ExtTemp4 = value; break;
                    default: console.log("Unknown Key: ", key);
                }
            }
			        // Voltage
			var point;

			if (chartVoltage) {
				point = chartVoltage.series[0].points[0];
				point.update(this.Voltage);
			}

			// Ampere
			if (chartAmpere) {
				point = chartAmpere.series[0].points[0];
				point.update(this.Current);
			}
			
			// Power
			if (chartPower) { // the chart may be destroyed
                var fwd = chartPower.series[0].points[0],
					refl = chartPower.series[1].points[0],
					vswr = chartPower.series[2].points[0];

				fwd.update(this.PowerForward, false);
				refl.update(this.PowerReflect, false);
				if (this.PowerVSWR < 0) {
					vswr.update(0, false);					
				} else {
					vswr.update(this.PowerVSWR, false);					
				}
				
				chartPower.redraw();
			}

			// Temp Intern
			if (chartIntTemp) { // the chart may be destroyed
                var intemp = chartIntTemp.series[0].points[0],
					outtemp = chartIntTemp.series[1].points[0],
					aprstemp= chartIntTemp.series[2].points[0];
					patemp= chartIntTemp.series[3].points[0];
					

				intemp.update(this.INTemp, false);
				outtemp.update(this.OUTTemp, false);
				aprstemp.update(this.APRSTemp, false);
				patemp.update(this.PATemp, false);
				
				chartIntTemp.redraw();
			}

			// Temp Extern
			if (chartExtTemp) { // the chart may be destroyed
               				var Exttemp1 = chartExtTemp.series[0].points[0],
					Exttemp2 = chartExtTemp.series[1].points[0],
					Exttemp3 = chartExtTemp.series[2].points[0];
					Exttemp4 = chartExtTemp.series[3].points[0];
					

				Exttemp1.update(this.ExtTemp1, false);
				Exttemp2.update(this.ExtTemp2, false);
				Exttemp3.update(this.ExtTemp3, false);
				Exttemp4.update(this.ExtTemp4, false);
				
				chartExtTemp.redraw();
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
	
    var TempgaugeOptions = {

        chart: {
            type: 'solidgauge'
        },

        tooltip: {
            enabled: false
        },

        plotOptions: {
            solidgauge: {
				innerRadius: '75%',
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
            min: 8,
            max: 16,
            title: {
                text: 'Voltage'
            },
            stops: [
                [0.1, '#DF5353'], // red
                [0.68, '#55BF3B'], // green
                [0.875, '#DF5353'] // red


			],
			plotBands: [{
                from: 0,
                to: 11,
                color: '#C02316',
                innerRadius: '100%',
                outerRadius: '103%'
            }, {
                from: 11,
                to: 14,
                color: '#00FF00',
                innerRadius: '100%',
                outerRadius: '103%'
			}, {
                from: 14,
                to: 16,
                color: '#C02316',
                innerRadius: '100%',
                outerRadius: '103%'
			}],
        },

        credits: {
            enabled: false
        },

        series: [{
            name: 'Voltage',
            data: [80],
            dataLabels: {
                format: '<div style="text-align:center"><span style="font-size:25px;color:' +
                    ((Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black') + '">{y:.1f}</span><br/>' +
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
            max: 8,
            title: {
                text: 'Ampere'
            },
            stops: [
                [0.1, '#55BF3B'], // green
                [0.5, '#DDDF0D'], // yellow
                [0.6, '#DF5353'] // red
            ],
			plotBands: [{
                from: 0,
                to: 6,
                color: '#00FF00',
                innerRadius: '100%',
                outerRadius: '103%'
            }, {
                from: 6,
                to: 7,
                color: '#FFFF00',
                innerRadius: '100%',
                outerRadius: '103%'
			}, {
                from: 7,
                to: 8,
                color: '#C02316',
                innerRadius: '100%',
                outerRadius: '103%'
			}],

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
	
    // The Intern Temp gauges
    chartIntTemp = Highcharts.chart('container-TempIntern', Highcharts.merge(TempgaugeOptions, {
        title: {
            text: null // 'Temperatures'
        },
		pane: [{
            startAngle: -45,
            endAngle: 45,
            background: null,
            center: ['10%', '100%'],
            size: 200
        }, {
            startAngle: -45,
            endAngle: 45,
            background: null,
            center: ['34%', '100%'],
            size: 200
        }, {
            startAngle: -45,
            endAngle: 45,
            background: null,
            center: ['57%', '100%'],
            size: 200
        }, {
            startAngle: -45,
            endAngle: 45,
            background: null,
            center: ['80%', '100%'],
            size: 200
        }],
		
		yAxis: [{
            min: -10,
            max: 70,
            title: {
                text: 'Inlet'
            },
            minorTickPosition: 'outside',
            tickPosition: 'outside',
			tickInterval: 20,
			minorTickInterval: 10,
            labels: {
                distance: 20
            },
			
            pane: 0,

            stops: [
                [0.1, '#55BF3B'], // green
                [0.7, '#DDDF0D'], // yellow
                [0.8, '#DF5353'] // red
            ],
			plotBands: [{
                from: -10,
                to: 50,
                color: '#00FF00',
                innerRadius: '100%',
                outerRadius: '103%'
            }, {
                from: 50,
                to: 60,
                color: '#FFFF00',
                innerRadius: '100%',
                outerRadius: '103%'
			}, {
                from: 60,
                to: 70,
                color: '#C02316',
                innerRadius: '100%',
                outerRadius: '103%'
			}],
        },{
            min: -10,
            max: 70,
            title: {
                text: 'Outlet'
            },
            pane: 1,
            minorTickPosition: 'outside',
            tickPosition: 'outside',
			tickInterval: 20,
			minorTickInterval: 10,
            labels: {
                distance: 20
            },

            stops: [
                [0.1, '#55BF3B'], // green
                [0.7, '#DDDF0D'], // yellow
                [0.8, '#DF5353'] // red
            ],
			plotBands: [{
                from: -10,
                to: 50,
                color: '#00FF00',
                innerRadius: '100%',
                outerRadius: '103%'
            }, {
                from: 50,
                to: 60,
                color: '#FFFF00',
                innerRadius: '100%',
                outerRadius: '103%'
			}, {
                from: 60,
                to: 70,
                color: '#C02316',
                innerRadius: '100%',
                outerRadius: '103%'
			}],
        },{
            min: -10,
            max: 70,
            title: {
                text: 'APRS TX'
            },
            pane: 2,
            minorTickPosition: 'outside',
            tickPosition: 'outside',
			tickInterval: 20,
			minorTickInterval: 10,
            labels: {
                distance: 20
            },

            stops: [
                [0.1, '#55BF3B'], // green
                [0.7, '#DDDF0D'], // yellow
                [0.8, '#DF5353'] // red
            ],
			plotBands: [{
                from: -10,
                to: 50,
                color: '#00FF00',
                innerRadius: '100%',
                outerRadius: '103%'
            }, {
                from: 50,
                to: 60,
                color: '#FFFF00',
                innerRadius: '100%',
                outerRadius: '103%'
			}, {
                from: 60,
                to: 70,
                color: '#C02316',
                innerRadius: '100%',
                outerRadius: '103%'
			}],
        },{
            min: -10,
            max: 70,
            title: {
                text: 'PA'
            },
            pane: 3,
            minorTickPosition: 'outside',
            tickPosition: 'outside',
			tickInterval: 20,
			minorTickInterval: 10,
            labels: {
                distance: 20
            },

            stops: [
                [0.1, '#55BF3B'], // green
                [0.7, '#DDDF0D'], // yellow
                [0.8, '#DF5353'] // red
            ],
			plotBands: [{
                from: -10,
                to: 50,
                color: '#00FF00',
                innerRadius: '100%',
                outerRadius: '103%'
            }, {
                from: 50,
                to: 60,
                color: '#FFFF00',
                innerRadius: '100%',
                outerRadius: '103%'
			}, {
                from: 60,
                to: 70,
                color: '#C02316',
                innerRadius: '100%',
                outerRadius: '103%'
			}],
		}],

        series: [{
            name: 'TempInlet',
            data: [1],
            yAxis: 0,
            dataLabels: {
                format: '<div style="text-align:center"><span style="font-size:20px;color:' +
                    ((Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black') + '">{y:.1f}</span><br/>' +
                       '<span style="font-size:12px;color:silver">°C</span></div>'
            },
            tooltip: {
                valueSuffix: ' C'
            }
        }, {
            name: 'TempOutlet',
            data: [1],
            yAxis: 1,
            dataLabels: {
                format: '<div style="text-align:center"><span style="font-size:20px;color:' +
                    ((Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black') + '">{y:.1f}</span><br/>' +
                       '<span style="font-size:12px;color:silver">°C</span></div>'
            },
            tooltip: {
                valueSuffix: ' C'
            }
        }, {
            name: 'TempAPRS',
            data: [1],
            yAxis: 2,
            dataLabels: {
                format: '<div style="text-align:center"><span style="font-size:20px;color:' +
                    ((Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black') + '">{y:.1f}</span><br/>' +
                       '<span style="font-size:12px;color:silver">°C</span></div>'
            },
            tooltip: {
                valueSuffix: ' C'
            }
        }, {
            name: 'TempPAlet',
            data: [1],
            yAxis: 3,
            dataLabels: {
                format: '<div style="text-align:center"><span style="font-size:20px;color:' +
                    ((Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black') + '">{y:.1f}</span><br/>' +
                       '<span style="font-size:12px;color:silver">°C</span></div>'
            },
            tooltip: {
                valueSuffix: ' C'
            }
		}]

    }));

    // The Extern Temp gauges
    chartExtTemp = Highcharts.chart('container-TempExtern', Highcharts.merge(TempgaugeOptions, {
        title: {
            text: null // 'Temperatures'
        },
		pane: [{
            startAngle: -45,
            endAngle: 45,
            background: null,
            center: ['10%', '100%'],
            size: 200
        }, {
            startAngle: -45,
            endAngle: 45,
            background: null,
            center: ['34%', '100%'],
            size: 200
        }, {
            startAngle: -45,
            endAngle: 45,
            background: null,
            center: ['57%', '100%'],
            size: 200
        }, {
            startAngle: -45,
            endAngle: 45,
            background: null,
            center: ['80%', '100%'],
            size: 200
        }],
		
		yAxis: [{
            min: -10,
            max: 70,
            title: {
                text: 'Extern 1'
            },
            minorTickPosition: 'outside',
            tickPosition: 'outside',
			tickInterval: 20,
			minorTickInterval: 10,
            labels: {
                distance: 20
            },
			
            pane: 0,

            stops: [
                [0.1, '#55BF3B'], // green
                [0.7, '#DDDF0D'], // yellow
                [0.8, '#DF5353'] // red
            ],
			plotBands: [{
                from: -10,
                to: 50,
                color: '#00FF00',
                innerRadius: '100%',
                outerRadius: '103%'
            }, {
                from: 50,
                to: 60,
                color: '#FFFF00',
                innerRadius: '100%',
                outerRadius: '103%'
			}, {
                from: 60,
                to: 70,
                color: '#C02316',
                innerRadius: '100%',
                outerRadius: '103%'
			}],
        },{
            min: -10,
            max: 70,
            title: {
                text: 'Extern 2'
            },
            pane: 1,
            minorTickPosition: 'outside',
            tickPosition: 'outside',
			tickInterval: 20,
			minorTickInterval: 10,
            labels: {
                distance: 20
            },

            stops: [
                [0.1, '#55BF3B'], // green
                [0.7, '#DDDF0D'], // yellow
                [0.8, '#DF5353'] // red
            ],
			plotBands: [{
                from: -10,
                to: 50,
                color: '#00FF00',
                innerRadius: '100%',
                outerRadius: '103%'
            }, {
                from: 50,
                to: 60,
                color: '#FFFF00',
                innerRadius: '100%',
                outerRadius: '103%'
			}, {
                from: 60,
                to: 70,
                color: '#C02316',
                innerRadius: '100%',
                outerRadius: '103%'
			}],
        },{
            min: -10,
            max: 70,
            title: {
                text: 'Extern 3'
            },
            pane: 2,
            minorTickPosition: 'outside',
            tickPosition: 'outside',
			tickInterval: 20,
			minorTickInterval: 10,
            labels: {
                distance: 20
            },

            stops: [
                [0.1, '#55BF3B'], // green
                [0.7, '#DDDF0D'], // yellow
                [0.8, '#DF5353'] // red
            ],
			plotBands: [{
                from: -10,
                to: 50,
                color: '#00FF00',
                innerRadius: '100%',
                outerRadius: '103%'
            }, {
                from: 50,
                to: 60,
                color: '#FFFF00',
                innerRadius: '100%',
                outerRadius: '103%'
			}, {
                from: 60,
                to: 70,
                color: '#C02316',
                innerRadius: '100%',
                outerRadius: '103%'
			}],
        },{
            min: -10,
            max: 70,
            title: {
                text: 'Extern 4'
            },
            pane: 3,
            minorTickPosition: 'outside',
            tickPosition: 'outside',
			tickInterval: 20,
			minorTickInterval: 10,
            labels: {
                distance: 20
            },

            stops: [
                [0.1, '#55BF3B'], // green
                [0.7, '#DDDF0D'], // yellow
                [0.8, '#DF5353'] // red
            ],
			plotBands: [{
                from: -10,
                to: 50,
                color: '#00FF00',
                innerRadius: '100%',
                outerRadius: '103%'
            }, {
                from: 50,
                to: 60,
                color: '#FFFF00',
                innerRadius: '100%',
                outerRadius: '103%'
			}, {
                from: 60,
                to: 70,
                color: '#C02316',
                innerRadius: '100%',
                outerRadius: '103%'
			}],
		}],

        series: [{
            name: 'TempInlet',
            data: [1],
            yAxis: 0,
            dataLabels: {
                format: '<div style="text-align:center"><span style="font-size:20px;color:' +
                    ((Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black') + '">{y:.1f}</span><br/>' +
                       '<span style="font-size:12px;color:silver">°C</span></div>'
            },
            tooltip: {
                valueSuffix: ' C'
            }
        }, {
            name: 'TempOutlet',
            data: [1],
            yAxis: 1,
            dataLabels: {
                format: '<div style="text-align:center"><span style="font-size:20px;color:' +
                    ((Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black') + '">{y:.1f}</span><br/>' +
                       '<span style="font-size:12px;color:silver">°C</span></div>'
            },
            tooltip: {
                valueSuffix: ' C'
            }
        }, {
            name: 'TempAPRS',
            data: [1],
            yAxis: 2,
            dataLabels: {
                format: '<div style="text-align:center"><span style="font-size:20px;color:' +
                    ((Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black') + '">{y:.1f}</span><br/>' +
                       '<span style="font-size:12px;color:silver">°C</span></div>'
            },
            tooltip: {
                valueSuffix: ' C'
            }
        }, {
            name: 'TempPAlet',
            data: [1],
            yAxis: 3,
            dataLabels: {
                format: '<div style="text-align:center"><span style="font-size:20px;color:' +
                    ((Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black') + '">{y:.1f}</span><br/>' +
                       '<span style="font-size:12px;color:silver">°C</span></div>'
            },
            tooltip: {
                valueSuffix: ' C'
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
            height: 150,
			spacingLeft: 1,
			spacingRight: 1
        },

        title: {
            text: null //'Power'
        },

        pane: [{
            startAngle: -45,
            endAngle: 45,
            background: null,
            center: ['16%', '145%'],
            size: 300
        }, {
            startAngle: -45,
            endAngle: 45,
            background: null,
            center: ['50%', '145%'],
            size: 300
        }, {
            startAngle: -45,
            endAngle: 45,
            background: null,
            center: ['84%', '145%'],
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
			tickInterval: 10,
			minorTickInterval: 5,

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
			tickInterval: 10,
			minorTickInterval: 5,


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
        }, {
		    min: 1,
            max: 3,
            minorTickPosition: 'outside',
            tickPosition: 'outside',
			tickInterval: 0.5,
			minorTickInterval: 0.1,
			

            labels: {
                rotation: 'auto',
                distance: 20
            },
            plotBands: [{
                from: 1,
                to: 1.4,
                color: '#00FF00',
                innerRadius: '100%',
                outerRadius: '105%'
            }, {
                from: 1.4,
                to: 2,
                color: '#FFFF00',
                innerRadius: '100%',
                outerRadius: '105%'
			}, {
                from: 2,
                to: 3,
                color: '#C02316',
                innerRadius: '100%',
                outerRadius: '105%'
			}],
            pane: 2,
            title: {
                text: 'VSWR<br/><span style="font-size:10px"></span>',
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
						((Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black') + '">{y:.1f}</span></div>',
                },
                dial: {
                    radius: '100%'
                }
            }
        },


        series: [{
            name: 'Fwd Power',
            data: [0],
            yAxis: 0
        }, {
            name: 'Ref Power',
            data: [0],
            yAxis: 1
        }, {
            name: 'VSWR',
            data: [1],
            yAxis: 2,
            dataLabels: {
                format: '<div style="text-align:center"><span style="font-size:25px;color:' +
                    ((Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black') + '">1:{y:.1f}</span></div>'
            },

        }]

    });

	var $document = $(document);
	var selector = '[data-rangeslider]';
	var $element = $(selector);

	 // For ie8 support
	var textContent = ('textContent' in document) ? 'textContent' : 'innerText';

	// Example functionality to demonstrate a value feedback
	function valueOutput(element) {
		var value = element.value;
		var output = element.parentNode.getElementsByTagName('output')[0] || element.parentNode.parentNode.getElementsByTagName('output')[0];
		output[textContent] = value;
	}

	$document.on('input', 'input[type="range"], ' + selector, function(e) {
		valueOutput(e.target);
	});

	// Basic rangeslider initialization
	$element.rangeslider({

		// Deactivate the feature detection
		polyfill: false,

		// Callback function
		onInit: function() {
			valueOutput(this.$element[0]);
		},


		// Callback function
		onSlideEnd: function(position, value) {
			console.log('value: ' + value);
			var req = {"SetPower": value};
			vm.send(req);
		}
	});


});
