'use strict';

(function () {
    // init
    $("[name='checkbox-led0']").bootstrapSwitch();
    $("[name='checkbox-led1']").bootstrapSwitch();
})();

var app = angular.module("inputDeviceServerApp", []); 

app.config(function($interpolateProvider){
    // to escape from Flask template expression
    $interpolateProvider.startSymbol('{[{').endSymbol('}]}');
});

app.controller("inputDeviceServerCtrl", function($scope, $timeout, $interval, $http) {    
    // init variables
    $scope.inputStatus = {
        keys: [1, 1],
        rotaryDegreeSpeed: 0,
        rotaryDegreeSum: 0,
        rotaryTickSpeed: 0,
        rotaryTickSum: 0,
        accelX: 0,
        accelY: 0,
        accelZ: 0,
        accelTapNum: 0,
    };
    $scope.rotaryWidth = 10;    // const
    $scope.rotaryPos = 0;      // from -45 to 45
    
    // start interval function
    var updateStatusTimer = $interval(updateStatus, 30);
    function updateStatus() {
        // Get Input Status from server
        $http({
            "method" : "GET",
            "url" : "getInputStatus"
        }).success(function(data, status, headers, config) {
            var previousTap = $scope.inputStatus.accelTapNum
            $scope.inputStatus = data;
            
            // modify data for key
            
            for (var i = 0; i < $scope.inputStatus.keys.length; i++) {
                if($scope.inputStatus.keys[i]==0){
                    $("#key-" + i).addClass("btn-primary");
                    $("#key-" + i).removeClass("btn-default");
                } else {
                    $("#key-" + i).removeClass("btn-primary");
                    $("#key-" + i).addClass("btn-default");
                }
            }
            
            // modify data for rotary
            $scope.rotaryPos = $scope.inputStatus.rotaryTickSpeed/2

            // modify data for tap
            if(previousTap!=0){
                $scope.inputStatus.accelTapNum = previousTap
            } else {
                if($scope.inputStatus.accelTapNum != 0){
                    $("#tap").addClass("red");                    
                    // reset tap num later
                    $timeout(function clearTap(){
                        $scope.inputStatus.accelTapNum = 0;
                        $("#tap").removeClass("red");
                    }, 2000);
                }
            }
            //console.log(data);
        }).error(function(data, status, headers, config) {
            console.log(status);
            $interval.cancel(updateStatusTimer);
        });
        

    }
 
    function setLED(gpio, out) {
        var parameter = {
            "gpio":gpio,
            "out":out
        }
        $http({
            "method" : "GET",
            "url" : "setLed",
            "params": parameter
        }).success(function(data, status, headers, config) {                       
            console.log(data);
        }).error(function(data, status, headers, config) {
            console.log(status);
        });
    }
    $('#checkbox-led0').on('switchChange.bootstrapSwitch', function () {
        var val = $('#checkbox-led0').prop('checked');
        
        if(val == true){
            setLED(0, 1);
        } else{
            setLED(0, 0);
        }
    });
    $('#checkbox-led1').on('switchChange.bootstrapSwitch', function () {
        var val = $('#checkbox-led1').prop('checked');
        
        if(val == true){
            setLED(1, 1);
        } else{
            setLED(1, 0);
        }
    });
});

