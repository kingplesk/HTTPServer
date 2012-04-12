
var Util = (function() {
    return {
        "$id": function(id) {
            return document.getElementById(id);
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

var Comet = (function() {

    var newComet = function() {
        setTimeout(function() {
            Ajax.send("http://test.localhost.lan:88/test?notify=" + getId(), callback);
        }, 0);
    };

    var callback = {
        success: function(responseText) {
            console.log('Comet Success: ' + responseText);

            newComet();
        },
        error: function(statusCode) { console.log('Failure: ' + statusCode); }
    }

    var getId = function() {
        return (new Date()).getTime() + "." + (Math.random() * 10000 + 10000);
    }

    var start = function(url) {
        newComet();
    }

    return {
        start: start
    };
})();
