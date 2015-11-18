var INTERVAL = 2000; //Time between requests
var locked = false; //Lock to avoid a request while a previous one is been handled
var dictionary = createDictionary();
var port=id_game=="memGame"?"8080":"9090";
var url="http://localhost:"+port;//http://192.168.1.33:8080"; //URL where the robot is listening

function extractResponseContent(response,key){
	return response.substring(response.indexOf(key)).replace(key,"").replace(/\s+/g,'');
}

function handleReqContent(command){
	var reqContent = "";
	switch(id_game){
		case "memGame":
			reqContent = translateNumber(command);  	
			if(reqContent>0 && reqContent<=2*NUMBER_PAIRS) toggleCard('card'+reqContent);
			break;
		case "diffGame":
            reqContent = command;
			if(reqContent !== -1 && (/image([0-9]+)_diff([0-9]+)/g).test(reqContent)) showDifference(reqContent);
			break;	
	}
	
}
function askForAction(){
	locked=true;
	var success=function(response){
		//console.out("ME RESPONDEN: "+response)
		var key = "";
		switch(phase){
			case 1:
				key="Level:";
				var level = translateNumber(extractResponseContent(response,key));  	
				if(level>0 && level<4) selectNumberOfCards(level);				
				break;
			case 2:
				key = "Requested content:";
				var command = extractResponseContent(response,key);
				if(command=="hint") getHint();
				else{
					handleReqContent(command);
				}	
				break;
			case 3:
				key="Restart:";
				var confirmation = extractResponseContent(response,key).toLowerCase();
				if(confirmation==="ok") reloadGame();
				break;
		}
		
		locked=false;
	};
	$.post(url,success);
}
function startPolling(){
	changePhase(1)
	setInterval(function () {if(!locked) askForAction()}, INTERVAL);
}
function createDictionary(){
	var dic = new Array();
	dic["one"]=1;	dic["two"]=2;	dic["three"]=3;	dic["four"]=4;
	dic["five"]=5;	dic["six"]=6;	dic["seven"]=7;	dic["eight"]=8;
	dic["nine"]=9;  dic["ten"]=10;  dic["eleven"]=11; dic["twelve"]=12;
	dic["easy_level"]=1; dic["medium_level"]=2; dic["hard_level"]=3;
	return dic;
}
function translateNumber(numberName){
	return dictionary[numberName.toLowerCase()];
}
function changePhase(next){
	phase=next;
	$.post(url,{Phase: next});
}
function sendSuccess(found){
	$.post(url,{partialSuccess: found});
}
