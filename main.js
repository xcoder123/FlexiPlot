// main var
var map;
var markers = [];
var infowindow = new google.maps.InfoWindow({});

// main init function
function initialize() {
    var myOptions = {		
		center:new google.maps.LatLng(51.508742,-0.120850),
        zoom: 12,
        mapTypeId: google.maps.MapTypeId.ROADMAP
    };

    map = new google.maps.Map(document.getElementById("map_canvas"), myOptions);

    newMarker(37.09024, -95.712891, 'trialhead0');
}

// custom functions
function setGMapCenter(lat, lng)
{
    map.setCenter(new google.maps.LatLng(lat, lng));
}

function setGMapZoom(zoom)
{
    map.setZoom(zoom);
}


function newMarker(_lat, _lng, _name)
{
        var lat = parseFloat(_lat );
        var lng = parseFloat( _lng );
        var name = _name;

        var myLatlng = new google.maps.LatLng(lat, lng);

        var contentString = "<html><body><div><p><h2>" + name + "</h2></p></div></body></html>";

        var marker = new google.maps.Marker({
            position: myLatlng,
            map: map,
            title: "Coordinates: " + lat + " , " + lng + " | Name: " + name
        });

        marker['infowindow'] = contentString;

        markers.push( marker );
        //marker.setMap(map);


        google.maps.event.addListener(marker, 'click', function() {
            infowindow.setContent(this['infowindow']);
            infowindow.open(map, this);
        });
}
