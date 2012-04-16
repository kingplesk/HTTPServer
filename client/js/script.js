
var Util = (function() {

    return {
        "$id": function(id) {
            return document.getElementById(id);
        },
        "getUniqueId" : function() {
            return (new Date()).getTime() + "." + (Math.random() * 10000 + 10000);
        },
        "hide": function(e) {
            e.style.display = "none";
        },
        "show": function(e) {
            e.style.display = "";
        },
        "html": function(e, content) {
            e.innerHTML = content;
        },
        "connect": function(event, e, callback) {
            if (e.addEventListener){
                e.addEventListener(event, callback, false);
            }
            else if (obj.attachEvent){
                e.attachEvent("on" + event, callback);
            }
        },
        "disconnect": function(event, e, callback) {
            if (e.removeEventListener) {
                e.removeEventListener (event, callback, false);
            }
            else if (e.detachEvent) {
                e.detachEvent ("on" + event, callback);
            }
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
    var slots = {}

    this.connect = function(slot, context) {
        context = context || null;
        var contextId = context + "", slotId = slot + "";

        if (!slots[contextId]) {
            var newSlots = {};
            newSlots[slotId] = {"slot": slot, "context": context};
            slots[contextId] = newSlots;
        }
        else {
            slots[contextId][slotId] = {"slot": slot, "context": context};
        }
    };

    this.disconnect = function(context, slot) {
        context = context || null;
        var contextId = context + "", slotId = slot + "";

        if (!slot) {
            slots[contextId] = null;
            return;
        }

        for (var slotIdx in slots[contextId]) {
            if (slotIdx === slotId) {
                slots[contextId][slotIdx] = null;
            }
        }
    };

    this.emit = function(data) {
        for (var i in slots) {
            for (var j in slots[i]) {
                slots[i][j].slot.call(slots[i][j].context, data);
            }
        }
    };

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
        // 3 - 7 sec.
        var timeout = (parseInt((Math.random() * 5), 10) + 3) * 1000;
        console.log("Timeout:", timeout);

        setTimeout(function() {
            var id = parseInt((Math.random() * 7), 10);
            instance["test" + id] && instance["test" + id].emit("test" + id);
            timer();
        }, timeout);
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

var TrayIcon = (function(Util, Comet) {
    var instance;
    var timer = null, timeout = 5000;
    var domContainer = Util.$id("TrayIcon"),
        domClose = Util.$id("TrayIconClose"),
        domPluginContainer = Util.$id("TrayIconPluginContainer");

    var show = function(content) {
        reset();

        Util.html(domPluginContainer, content);
        Util.show(domContainer);

        timer = setTimeout(function() {
            hide();
        }, timeout);
    };

    var reset = function() {
        if (timer) clearTimeout(timer);
        timer = null;
    }

    var hide = function() {
        Util.hide(domContainer);
        reset();
    }

    var close = function(e) {
        e.preventDefault;
        hide();
    }

    Util.connect("click", domClose, function(e) {
        hide();
    });

    TrayIcon = function TrayIcon() {
        if (instance) {
            return instance;
        }

        instance = this;

        this.show = show;
    };

    return new TrayIcon();

})(Util, Comet);

var PluginTest0 = (function(Comet, TrayIcon/*, Clipboard, Widget*/) {
    var trayIconContent = function(data) {
        var content = '<div style="background-color:red; height: 100%; font-weight:bold;">PluginTest0 - ' + data + '</div>';
        TrayIcon.show(content);
    };

    Comet.test0.connect(trayIconContent);
    Comet.test0.connect(console.log);

    //Clipboard.addIcon(/* html icon */ icon, /* event function callback */ callback);

})(Comet, TrayIcon);


//
Comet.test1.connect(console.log);
Comet.test1.connect(TrayIcon.show);
//
Comet.test2.connect(console.log);
Comet.test2.connect(TrayIcon.show);
//
Comet.test3.connect(console.log);
Comet.test3.connect(TrayIcon.show);
//
Comet.test4.connect(console.log);
Comet.test4.connect(TrayIcon.show);
//
Comet.test5.connect(console.log);
Comet.test5.connect(console.info);
Comet.test5.connect(TrayIcon.show);
//
//Comet.test6.connect(console.log);
//Comet.test6.connect(TrayIcon.show);
