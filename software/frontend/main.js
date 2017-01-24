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
			var point,

			if (chartVoltage) {
				point = chartVoltage.series[0].points[0];

				point.update(this.Voltage);
				console.log(this.Voltage);
			}

			// Ampere
			if (chartAmpere) {
				point = chartAmpere.series[0].points[0];

				point.update(this.Ampere);
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
//        restart: function(event) {
//            this.send("Restart");
//        },
        
/*        send_message: function(event)  {
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


