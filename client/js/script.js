
var Util = (function() {
    return {
        "$id": function(id) {
            return document.getElementById(id);
        },
        "getUniqueId" : function() {
            return (new Date()).getTime() + "." + (Math.random() * 10000 + 10000);
        }
    };

})();

var Ajax = (function() {
    var createXhr = function() {
        var created = null, methods = [
            function() { return new XMLHttpRequest(); },
            function() { return new ActiveXObject('Msxml2.XMLHTTP'); },
            function() { return new ActiveXObject('Microsoft.XMLHTTP'); }
        ];

        for(var i=0, ilen=methods.length; i<ilen; i++) {
            try { created = methods[i](); }
            catch(e) { continue; }

            createXhr = methods[i];
            return created;
        }
        throw new Error('Could not create an XHR object.');
    };

    var send = function(url, method, callback, postVars) {
        var xhr = createXhr();
        if (xhr) {
            xhr.onreadystatechange = function() {
                if(xhr.readyState !== 4) return;

                var context = null;
                if (xhr.status === 200) callback.success.call(context, xhr.responseText, xhr.responseXML);
                else callback.error.call(context, xhr.status);
            };
            xhr.open(method, url, true);
            xhr.setRequestHeader('Content-Type', 'application/json');

            if(method !== 'POST') postVars = null;
            xhr.send(postVars);
        }
    };

    return {
        send: function(url, callback, method, postVars) {
            method = method || 'POST';
            postVars = postVars || null;

            send(url, method, callback, postVars);
        }
    };

})()

var Signal = function() {
    this.slots = {};
    this.add = function(slot, context) {
        context = context || null;
        var contextId = context + "", slotId = slot + "";

        if (!this.slots[contextId]) {
            var newSlots = {};
            newSlots[slotId] = {"slot": slot, "context": context};
            this.slots[contextId] = newSlots;
        }
        else {
            this.slots[contextId][slotId] = {"slot": slot, "context": context};
        }
    };
};

Signal.prototype = {
    connect : function(slot, context) {
        this.add(slot, context);
    },
    disconnect: function(context, slot) {

    },
    emit : function(data) {
        for (var i in this.slots) {
            for (var j in this.slots[i]) {
                this.slots[i][j].slot.call(this.slots[i][j].context, data);
            }
        }
    }
};

var Comet = (function(Ajax, Util, Signal) {
    var instance;
    var isStarted = false;
    var newComet = function(sleep) {
        sleep = sleep || 0;
        setTimeout(function() {
            Ajax.send("http://test.localhost.lan:88/test?notify=" + Util.getUniqueId(), callback);
        }, sleep);
    };

    var callback = {
        success: function(responseText) {
            console.log('Comet Success: ' + responseText);

            //instance["test" + id] && instance["test" + id].emit("test" + id);

            newComet();
        },
        error: function(statusCode) { console.log('Error: ' + statusCode); }
    };


    var start = function(url) {
        if (isStarted) return;
        isStarted = true;
        newComet();
    };

    // BEGIN: TestTimer for Signal implmentation
    var timer = function() {
        setTimeout(function() {
            var id = parseInt((Math.random() * 7), 10);
            instance["test" + id] && instance["test" + id].emit("test" + id);

            timer();
        }, 5000);
    };

    timer();
    // END: TestTimer

    Comet = function Comet() {
        if (instance) {
            return instance;
        }

        instance = this;

        this.start = start;

        this.test0 = new Signal();
        this.test1 = new Signal();
        this.test2 = new Signal();
        this.test3 = new Signal();
        this.test4 = new Signal();
        this.test5 = new Signal();
        this.test6 = new Signal();
        this.test7 = new Signal();
    };

    return new Comet();

})(Ajax, Util, Signal);

var Notify = (function(Util, Comet) {
    var timer = null, timeout = 5000;
    var show = function(notifier) {
        next(timeout);
    };

    var next = function() {
        timer = setTimeout(function() {
            //hide content
        }, timeout);
    };

    Comet.test0.connect(console.log);
    Comet.test1.connect(console.log);
    Comet.test2.connect(console.log);
    Comet.test3.connect(console.log);
    Comet.test4.connect(console.log);
    Comet.test5.connect(console.log);
    Comet.test5.connect(console.info);
    Comet.test6.connect(console.log);

})(Util, Comet);
