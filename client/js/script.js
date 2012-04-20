
var Util = (function() {

    return {
        "$id": function(id) {
            return document.getElementById(id);
        },
        "$tag": function(tag, context) {
            context = context || document;
            return context.getElementsByTagName(tag);
        },
        "getOffset": function(el) {
            var offset = {t: 0, l: 0};

            while (el) {
                if (el.offsetTop !== undefined)
                  offset.t  += parseInt(el.offsetTop);
                if (el.offsetLeft !== undefined)
                  offset.l += parseInt(el.offsetLeft);

                el = el.parentOffset;
            }

            return offset;
        },
        "getUniqueId" : function() {
            return (new Date()).getTime() + "." + (Math.random() * 10000 + 10000);
        },
        "hide": function(el) {
            el.style.display = "none";
        },
        "show": function(el) {
            el.style.display = "";
        },
        "html": function(el, content) {
            el.innerHTML = content;
        },
        "hasClass": function(el, className) {
            return (" " + el.className + " ").indexOf(" " + className + " ") !== -1;

        },
        "addClass": function(el, className) {
            if (this.hasClass(el, className)) {
                return;
            }

            el.className += " " + className
        },
        "removeClass": function(el, className) {
            if (!this.hasClass(el, className)) {
                return;
            }

            var splitted = (" " + el.className + " ").split(" " + className + " ");
            el.className = splitted.join(" ");
        },
        "toggleClass": function(el, className, force) {
            if (force) {
                this.addClass(el, className);
            }
            else {
                this.removeClass(el, className);
            }
        },
        "connect": function(event, e, callback) {
            if (e.addEventListener){
                e.addEventListener(event, callback, false);
            }
            else if (obj.attachEvent){
                e.attachEvent("on" + event, callback);
            }
        },
        "disconnect": function(event, el, callback) {
            if (el.removeEventListener) {
                el.removeEventListener (event, callback, false);
            }
            else if (el.detachEvent) {
                el.detachEvent ("on" + event, callback);
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

    this.emit = function() {
        var args = Array.prototype.slice.call(arguments);
        for (var i in slots) {
            for (var j in slots[i]) {
                slots[i][j].slot.apply(slots[i][j].context, args);
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
        //console.log("Timeout:", timeout);

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

    Util.connect("click", domClose, function(e) {
         e.preventDefault;
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

var Selector = (function(Util, Signal) {
    var selecting = false, start = {x: null, y: null}, current = {x: null, y: null, w: null, h: null, t: null, l:null},
        el = document, domRectangle, suffix = Util.getUniqueId().replace(/\./g, "-"), rectangleId = "selector-rectangle-" + suffix,
        prefixId = "selector-item-" + suffix + "-", signals = { checkIsSelected: new Signal(), start: new Signal(), stop: new Signal() }, snapshot = [];

    var reset = function() {
        selecting = false;
        signals.stop.emit();
        snapshot = [];

        if (domRectangle) {
            Util.hide(domRectangle);
        }
        else {
            domRectangle = document.createElement("div");

            domRectangle.id = rectangleId;
            domRectangle.style.position = "absolute";
            domRectangle.style.border = "1px solid black";
            domRectangle.style.display = "none";
            domRectangle.style.width = "1px";
            domRectangle.style.height = "1px";

            Util.$tag("body")[0].appendChild(domRectangle);
        }
    };

    var onMouseMove = function(e) {
        e.preventDefault();

        current.x = document.all ? window.event.clientX : e.pageX;
        current.y = document.all ? window.event.clientY : e.pageY;

        if (selecting) {
            selectedArea();
        }
    };

    var onMouseDown = function(e) {
        e.preventDefault();

        signals.start.emit();
        selecting = true;

        start.x = current.x;
        start.y = current.y;

        selectedArea();

        Util.show(domRectangle);
    }

    var onMouseUp = function(e) {
        e.preventDefault();
        reset();
    }

    var selectedArea = function() {
        if ((current.x - start.x) < 0) {
            current.l = current.x;
            current.w = -1 * (current.x - start.x);
        }
        else {
            current.l = start.x;
            current.w = current.x - start.x;
        }

        if ((current.y - start.y) < 0) {
            current.t = current.y;
            current.h = -1 * (current.y - start.y);
        }
        else {
            current.t = start.y;
            current.h = current.y - start.y;
        }

        drawRectangle(current.t, current.l, current.w, current.h);
        signals.checkIsSelected.emit(current.t, current.l, current.w, current.h);
    };

    var drawRectangle = function(t, l, w, h) {
        domRectangle.style.top    = t + "px";
        domRectangle.style.left   = l + "px";
        domRectangle.style.width  = w + "px";
        domRectangle.style.height = h + "px";
    };

    var getSnapshotOfElement = function(el) {
        return snapshot[getId(el.id)];
    };

    var getId = function(id) {
        return id.substr(id.lastIndexOf + 1);
    };

    reset();

    Util.connect("mousedown", el, onMouseDown);
    Util.connect("mousemove", el, onMouseMove);
    Util.connect("mouseup", el, onMouseUp);

    Selector = function Selector(el, callbacks) {
        var selectables = [], selectableClass = "selectable", childNodes = el.childNodes,
            childNode, nullFunction = function(selected) { /*console.log("selected", selected)*/ },
            selectedClass = "state-selected";

        //add external callbacks
        callbacks = callbacks || { onSelected: nullFunction, onSelectedItem: nullFunction};
        callbacks.onSelected = callbacks.onSelected || nullFunction;
        callbacks.onSelectedItem = callbacks.onSelectItem || nullFunction;

        //collect selectable elements on first layer
        for (var i = 0, j = 0, ilen = childNodes.length; i < ilen; i++) {
            childNode = childNodes[i];
            if (childNode.nodeType == 1 && childNode.className == selectableClass) {
                childNode.id = prefixId + (j++);
                selectables.push(childNode);
            }
        }

        signals.checkIsSelected.connect(function(t, l, w, h) {
            var selectable = null, selected = [], offset, snapshotEl;

            for (var i = 0, ilen = selectables.length; i < ilen; i++) {
                selectable = selectables[i];
                snapshotEl = getSnapshotOfElement(selectable);
                offset = Util.getOffset(selectable);

                if ( !(t <= offset.t + selectable.offsetHeight ) ) { Util.toggleClass(selectable, selectedClass, snapshotEl.isSelected); continue; }
                if ( !(t+h >= offset.t) ) { Util.toggleClass(selectable, selectedClass, snapshotEl.isSelected); continue; }
                if ( !(l <= offset.l + selectable.offsetWidth) ) { Util.toggleClass(selectable, selectedClass, snapshotEl.isSelected); continue; }
                if ( !(l+w >= offset.l) ) { Util.toggleClass(selectable, selectedClass, snapshotEl.isSelected); continue; }

                Util.toggleClass(selectable, selectedClass, !snapshotEl.isSelected);

                selected.push(selectable);
                callbacks.onSelectedItem(selectable);
            }

            if (selected.length > 0) {
                callbacks.onSelected(selected);
            };
        });

        signals.start.connect(function() {
            for (var i = 0, ilen = selectables.length; i < ilen; i++) {
                snapshot[getId(selectables[i].id)] = { el: selectables[i], isSelected: Util.hasClass(selectables[i], selectedClass)};
            }
        });
    };

    return Selector;
})(Util, Signal);

var test = new Selector(Util.$id("selectRoot"));



var PluginTest0 = (function(Comet, TrayIcon/*, Clipboard, Widget*/) {
    var trayIconContent = function(data) {
        var content = '<div style="background-color:red; height: 100%; font-weight:bold;">PluginTest0 - ' + data + '</div>';
        TrayIcon.show(content);
    };

    Comet.test0.connect(trayIconContent);
    //Comet.test0.connect(console.log);

    //Clipboard.addIcon(/* html icon */ icon, /* event function callback */ callback);

})(Comet, TrayIcon);


//
//Comet.test1.connect(console.log);
Comet.test1.connect(TrayIcon.show);
//
//Comet.test2.connect(console.log);
Comet.test2.connect(TrayIcon.show);
//
//Comet.test3.connect(console.log);
Comet.test3.connect(TrayIcon.show);
//
//Comet.test4.connect(console.log);
Comet.test4.connect(TrayIcon.show);
//
//Comet.test5.connect(console.log);
//Comet.test5.connect(console.info);
Comet.test5.connect(TrayIcon.show);
//
//Comet.test6.connect(console.log);
//Comet.test6.connect(TrayIcon.show);
