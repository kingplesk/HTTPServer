//@todo: init time branching, sandbox
var Ajax = (function() {
  var activeXids = [
    'MSXML2.XMLHTTP.3.0',
    'MSXML2.XMLHTTP',
    'Microsoft.XMLHTTP'
  ];

  function newXhr() {
    if (typeof XMLHttpRequest === 'function') {
      return new XMLHttpRequest();
    }
    else {
      for (var i=0; i<activeXids.length; i++) {
        try {
          return new ActiveXObject(activeXids[i]);
          break;
        } catch (e) {}
      }
    }
  }

  function send(url, method, callback, context) {
    var xhr = newXhr();
    if (xhr) {
      xhr.onreadystatechange = function() {
        if (xhr.readyState === 4 && xhr.status === 200) {
          callback.call(context, xhr.responseText, xhr);
        }
        else {
        }
      };
      xhr.open(method, url, true); 
      xhr.send(null);
    }
  }

  return {
    send : function(url, callback, context, method) {
      method = method || 'POST';
      context = context || null;

      send(url, method, callback, context);
    }
  }
})();

