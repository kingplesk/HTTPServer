console.log("Util");

var Util = (function() {

    var eventWrapper = function(callback, context) {
        context = context || null;

        return function(e) {
            var src;

            e = e || window.event;
            src = e.target || e.srcElement;

            if (typeof e.stopPropagation === "function") {
                e.stopPropagation();
            }
            if (typeof e.cancelBubble !== "undefined") {
                e.cancelBubble = true;
            }

            if (typeof e.preventDefault === "function") {
                e.preventDefault();
            }
            if (typeof e.returnValue !== "undefined") {
                e.returnValue = false;
            }

            callback.apply(context, [e, src]);
        }
    };

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
        "scrollPosition": function() {
            var x = 0, y = 0;

            if( typeof( window.pageYOffset ) == 'number' ) {
                //Netscape compliant
                y = window.pageYOffset;
                x = window.pageXOffset;
            }
            else if( document.body && ( document.body.scrollLeft || document.body.scrollTop ) ) {
                //DOM compliant
                y = document.body.scrollTop;
                x = document.body.scrollLeft;
            }
            else if (window.scrollY) {
                x = window.scrollX;
                y = window.scrollY;
            }
            else if( document.documentElement && ( document.documentElement.scrollLeft || document.documentElement.scrollTop ) ) {
                //IE6 standards compliant mode
                y = document.documentElement.scrollTop;
                x = document.documentElement.scrollLeft;
            }

            return { "x": x, "y": y };
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

            var whitespace = "";
            if (el.className != "" ) {
                whitespace = " ";
            }

            el.className += whitespace + className;

        },
        "removeClass": function(el, className) {
            if (!this.hasClass(el, className)) {
                return;
            }
            //             className->    rm className->                           rm double spaces->   trim
            el.className = (el.className).replace(new RegExp(className, "g"), " ").replace(/\s+/g, " ").replace(/^\s*(.*?)\s*$/, "$1");
        },
        "toggleClass": function(el, classNames, forces) {
            var tmpForce;
            if (!this.isArray(classNames)) {
                classNames = [classNames];
            }
            if (!this.isArray(forces)) {
                forces = [forces];
            }

            for (var i = 0, ilen = classNames.length; i < ilen; i++) {
                tmpForce = forces[i] !== undefined ? forces[i] : !this.hasClass(el, classNames[i]);

                if (tmpForce) {
                    this.addClass(el, classNames[i]);
                }
                else {
                    this.removeClass(el, classNames[i]);
                }
            }
        },
        "is": function(value, type) {
            return Object.prototype.toString.call(value) === '[object ' + type + ']';
        },
        "isArray": function(value) {
            return this.is(value, 'Array');
        },
        "isObject": function(value) {
            return this.is(value, 'Object');
        },
        "isFunction": function(value) {
            return this.is(value, 'Function');
        },
        "isNumber": function(value) {
            return this.is(value, 'Number');
        },
        "isUndefined": function(value) {
            return this.is(value, 'Undefined');
        },
        "isBoolean": function(value) {
            return this.is(value, 'Boolean');
        },
        "isRegExp": function(value) {
            return this.is(value, 'RegExp');
        },
        "isElement": function(value) {
            return /^\[object HTML.*\]$/i.test(Object.prototype.toString.call(value));
        },
        "connect": function(event, el, callback, context) {
            context = context || null;

            if (el.addEventListener){
                el.addEventListener(event, eventWrapper(callback, context), false);
            }
            else if (el.attachEvent){
                el.attachEvent("on" + event, eventWrapper(callback, context));
            }
        },
        "disconnect": function(event, el, callback, context) {
            context = context || null;

            if (el.removeEventListener) {
                el.removeEventListener (event, eventWrapper(callback, context), false);
            }
            else if (el.detachEvent) {
                el.detachEvent ("on" + event, eventWrapper(callback, context));
            }
        }
    };

})();

console.log("Ajax");

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

console.log("Signal");

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

console.log("Comet");

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

console.log("TrayIcon");

var TrayIcon = (function(Util, Comet) {
    var instance;
    var timer = null, timeout = 5000,
        domContainer = Util.$id("TrayIcon"),
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
    };

    var hide = function() {
        Util.hide(domContainer);
        reset();
    };

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

console.log("Selector")

var Selector = (function(Util, Signal) {
    var selecting = false, start = { x: null, y: null },
        current = { x: null, y: null, w: null, h: null, t: null, l:null },
        el = document, domRectangle, suffix = Util.getUniqueId().replace(/\./g, "-"),
        rectangleId = "selector-rectangle-" + suffix, prefixId = "selector-item-" + suffix + "-",
        signals = { checkIsSelected: new Signal(), start: new Signal(), stop: new Signal() };

    var reset = function() {
        selecting = false;
        signals.stop.emit();

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
        var scrollPosition = { "x": 0, "y": 0 };
        if (document.all) {
            scrollPosition = Util.scrollPosition();
        }

        current.x = document.all ? (window.event.clientX + scrollPosition.x) : e.pageX;
        current.y = document.all ? (window.event.clientY + scrollPosition.y) : e.pageY;

        if (selecting) {
            selectedArea();
        }
    };

    var onMouseDown = function(e) {
        signals.start.emit(!!e.ctrlKey);

        selecting = true;

        start.x = current.x;
        start.y = current.y;

        selectedArea();

        Util.show(domRectangle);
    };

    var onMouseUp = function(e) {
        reset();
    };

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

    var getIdx = function(id) {
        return parseInt(id.substr(id.lastIndexOf("-") + 1), 10);
    };

    var getId = function(idx) {
        return prefixId + idx;
    }

    reset();

    Util.connect("mousemove", el, onMouseMove);
    Util.connect("mouseup", el, onMouseUp);

    Selector = function Selector(el) {
        var selectables = [], selectableClass = "selectable", childNodes = el.childNodes,
            childNode, selectedClass = "state-selected", snapshot = [], ctrlKeyPressed = false;

        var getSnapshotOfElement = function(el) {
            return snapshot[getIdx(el.id)];
        };

        var getStage = function() {
            var stage = [], ids = [];
            for (var i = 0, ilen = selectables.length; i < ilen; i++) {
                if (Util.hasClass(selectables[i], selectedClass)) {
                    stage.push(selectables[i]);
                    ids.push(getIdx(selectables[i].id));
                }
            }

            return { stage: stage, items: ids };
        };

        var iterator = function(items, callbackItem) {
            var args = Array.prototype.slice.call(arguments);

            if (args.length === 0) { return; }
            if (items.length === 0) { return; }

            args.splice(0, 2);

            for (var i = 0, ilen = items.length, el; i < ilen; i++) {
                el = Util.isNumber(items[i]) ? selectables[items[i]] : items[i];

                callbackItem.apply(null, [el].concat(args));
                Util.removeClass(items[i], selectedClass);
            }

            snapshot = [];
        };

        //init signals, callback and listener
        this.onSelected = new Signal();
        this.onSelectedItem = new Signal();

        /**
         *  possible usecases for use of commit fct:
         *      fct: item callback function
         *          el: selected dom element
         *          param1-N: custom parameters for fct
         *
         *      object: setter with items and parameters array
         *          abstract:   { items: [N-1], params; [N-1] }
         *          example:    { items: [1,3,5,7,11,13,17], params: ['#ff00ee'] }
         *
         *      void commit(fct [, param1 [, param2 [, paramN]]])    -> fct(el [, param1 [, param2 [, paramN]]])
         *      void commit(object, fct)                             -> fct(el [, param1 [, param2 [, paramN]]])
         *      object commit()                                      -> n.a.
         */

        function commitObject(object, callbackItem) {
            var args = object.params || [];
            args.splice(0, 0, callbackItem);

            iterator.apply(null, [object.items].concat(args));
        }

        function commitNull() {
            return { items: getStage().items, params: [] };
        }

        function commit(callbackItem) {
            var staging = getStage().stage, args = Array.prototype.slice.call(arguments);
            iterator.apply(null, [staging].concat(args));
        }

        this.commit = function(fctOrObject) {
            var args = Array.prototype.slice.call(arguments);

            if (args.length === 0) {
                return commitNull();
            }
            if (Util.isObject(fctOrObject)) {
                return commitObject.apply(null, args);
            }

            return commit.apply(null, args);
        };

        this.reset = function(callbackItem) {
            callbackItem = callbackItem || function() {};
            var args = Array.prototype.slice.call(arguments);
            args.splice(0, 1, callbackItem);

            iterator.apply(null, [selectables].concat(args));
        };

        Util.connect("mousedown", el, onMouseDown);

        //collect selectable elements on first layer
        for (var i = 0, j = 0, ilen = childNodes.length; i < ilen; i++) {
            childNode = childNodes[i];
            if (childNode.nodeType == 1 && childNode.className == selectableClass) {
                childNode.id = prefixId + j;
                selectables[j] = childNode;
                j++;
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

                Util.toggleClass(selectable, selectedClass, !snapshotEl.isSelected || isCtrlKeyPressed);

                selected.push(selectable);
                this.onSelectedItem.emit(selectable);
            }

            if (selected.length > 0) {
                this.onSelected.emit(selected);
            };
        }, this);

        signals.start.connect(function(isPressed) {
            isCtrlKeyPressed = isPressed;

            for (var i = 0, ilen = selectables.length, idx, isStaging; i < ilen; i++) {
                idx = getIdx(selectables[i].id);
                snapshot[idx] = { idx: idx, id: selectables[i].id, el: selectables[i], isSelected: Util.hasClass(selectables[i], selectedClass)};
            }
        });

        signals.stop.connect(function() {
            snapshot = [];
        });
    };

    return Selector;
})(Util, Signal);


var selector = new Selector(Util.$id("selectRoot"));




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
