
angular.module('app.services', [])

    .service('DataService', [ function () {
      var com = {};
      com.exhibit = {} ;
      com.exhibit.data = {};
      return {
        setData: function(val) {
          com.exhibit.data = val;
        },
        getData: function() {
          return com.exhibit.data;
        }
      }
    }])

    .service('AppService', ['$q', 'ParseConfiguration',
        function ($q, ParseConfiguration) {
          return {
            //Will query parse for Exhibits
            queryExhibits: function (callback) {
              var ExhibitList = Parse.Object.extend("Exhibits");
              var query = new Parse.Query(ExhibitList);

              query.find({
                success: function(results) {
                  console.log("Successfully retrieved " + results.length + " exhibits.");
                  // Do something with the returned Parse.Object values

                  //Create empty list of exhibits in order to push queried exhibits to
                  var listOfExhibits = [];

                  //Iterate through results of query and add to array^^
                  for (var i = 0; i < results.length; i++) {
                    var object = results[i];

                    var objectID = object.id;
                    var title = object.get('title');
                    var address = object.get('address');
                    var location = object.get('location');
                    var thumbnail = object.get('thumbnail');
                    var sphere = object.get('sphere');
                    var isPicture = object.get('isPicture');
                    var beds = object.get('beds');
                    var baths = object.get('baths');
                    var pets = object.get('pets');
                    var price = object.get('price');
                    var sqft = object.get('sqft');
                    var city = object.get('city');
                    var zip = object.get('zip');

                    //If no thumbnail set
                    if(typeof thumbnail === 'undefined'){
                      thumbnail = {_url: "http://files.parsetfss.com/05724cba-8bc9-4ddc-9a62-90b6e75efa1a/tfss-a22124f5-1557-4d7f-b630-f0246dcb9cc1-06.jpg"}
                    }

                    var exhibit = {objectID: objectID, title:title, address:address, location:location,
                                  thumbnail:thumbnail, sphere:sphere, isPicture: isPicture, beds:beds,
                                  baths:baths, pets:pets, price:price, sqft: sqft, city: city, zip: zip};

                    //Add to list of exhibits
                    listOfExhibits.push(exhibit);
                  }
                  console.log("Calling back...");
                  callback(listOfExhibits);
                },
                error: function(error) {
                  alert("Error: " + error.code + " " + error.message);
                }
              });
            }

          }
        }]);
