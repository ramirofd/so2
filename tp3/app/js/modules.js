document.addEventListener('DOMContentLoaded', function() {
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
        year = document.getElementById('year');
        day = document.getElementById('day');
        var url = 'http://192.168.0.71/cgi-bin/goes.cgi?year='+year.value+'&day='+day.value;
        xhr = getXmlHttpRequestObject();
        xhr.onreadystatechange = evenHandler;
        // asynchronous requests
        xhr.open("GET", url, true);
        // Send the request over the network
        xhr.send(null);
        var x = document.getElementById("preLoader");
        x.style.display = "block";
    };

    function evenHandler()
    {
        // Check response is ready or not
        if(xhr.readyState == 4 && xhr.status == 200)
        {
            tabledata = document.getElementById('tableData');
	        tabledata.innerHTML = xhr.responseText;
            var x = document.getElementById("preLoader");
            x.style.display = "none";
            var x = document.getElementById("resultCard");
            x.style.display = "block";
        }
    }
    
 }, false);