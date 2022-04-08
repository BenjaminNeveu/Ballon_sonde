function envoiTrame() {
    $.ajax({
        url: "/envoiTrame",
        type: 'GET',
        success:
                function (donnees, status, xhr)
                {
                    console.log(donnees);
                },
        error:
                function (xhr, status, error)
                {
                    console.log("param : " + JSON.stringify(xhr));
                    console.log("status : " + status);
                    console.log("error : " + error);
                }
    });
}

function recupererDonnee() {
    $.ajax({
        url: "/recupererDonnee",
        type: 'POST',
        success:
                function (donnees, status, xhr)
                {
                    
                    
                    
                    /*console.log(donnees);
                     const json = this.donnees;
                     const obj = JSON.parse(json);
                     
                     $("#longitude").text(obj.longitude);
                     $("#latitude").text(obj.latitude);
                     $("#altitude").text(obj.altitude);
                     $("#radiation").text(obj.radiation);
                     $("#temperature").text(obj.temperature);
                     $("#humidite").text(obj.humidite);
                     $("#pression").text(obj.pression);
                     */
                },
        error:
                function (xhr, status, error)
                {
                    console.log("param : " + JSON.stringify(xhr));
                    console.log("status : " + status);
                    console.log("error : " + error);
                }
    });
    /*
     var xhr = new XMLHttpRequest();
     xhr.onreadystatechange = function ()
     {
     if (this.readyState == 4 && this.status == 200)
     {
     console.log(this.responseText);
     const json = this.responseText;
     const obj = JSON.parse(json);
     
     $("#longitude").text(obj.longitude);
     $("#latitude").text(obj.latitude);
     $("#altitude").text(obj.altitude);
     $("#radiation").text(obj.radiation);
     $("#temperature").text(obj.temperature);
     $("#humidite").text(obj.humidite);
     $("#pression").text(obj.pression);
     
     }
     };
     xhr.open("POST", "recupererDonnee", true);
     xhr.send();*/
}

setInterval(function getData() {
    recupererDonnee();
}, 30000);

