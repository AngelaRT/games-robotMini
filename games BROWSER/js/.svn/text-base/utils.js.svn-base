var NUMBER_PAIRS=4;
var NUMBER_COLS=4;
var NUMBER_DIFFERENT_IMAGES=2;
var NUMBER_DIFFS = 99;
var countClicks=0;
var countSuccess=0;
var possiblePair=["",""];
var phase = 1; // Phases of the game: 1=select level; 2=play; 3=restart game
var solution = new Array();
var idImg = 0;

function toggleCard(idDiv){
	/*Shows the front or the back of the card*/
	if(!$('#'+idDiv).hasClass('solved')){
		$('#'+idDiv+' img.backCard').hasClass('oculta') ? $('#'+idDiv+' img.backCard').removeClass('oculta').addClass('visible') : $('#'+idDiv+' img.backCard').removeClass('visible').addClass('oculta');
		$('#'+idDiv+' img.frontCard').hasClass('oculta') ? $('#'+idDiv+' img.frontCard').removeClass('oculta').addClass('visible') : $('#'+idDiv+' img.frontCard').removeClass('visible').addClass('oculta');
	
		if(++countClicks===2){
			possiblePair[1]=$('#'+idDiv+' img.frontCard');
			countClicks=0;
			if(possiblePair[0].selector!==possiblePair[1].selector){
				/*If it is the second click: 
					* Check if the shown cards are the same
					* Hides the front cards that are not solved*/
				if ($(possiblePair[0]).attr('src')	=== $(possiblePair[1]).attr('src')){
					countSuccess++;
					for(var i=0; i<possiblePair.length; i++){
						$(possiblePair[i]).parent('.card').addClass('solved');
					}
					updateSolution(possiblePair);
					sendSuccess(true);
				}else{
					sendSuccess(false);
				}
				setTimeout(function(){
					$('.card').each(function(i,card){
						if(!$(card).hasClass('solved')){
							$(card).find('img.frontCard').removeClass('visible').addClass('oculta');
							$(card).find('img.backCard').removeClass('oculta').addClass('visible');
						}
					});
					if(countSuccess===NUMBER_PAIRS){showFinishGame();}
				},1000);
			}		
		}else{
			possiblePair[0]=$('#'+idDiv+' img.frontCard');
		}
	}	
}
function showDifference(idSol){
	if(!$('#'+idSol).hasClass('solved')){
		$('#'+idSol).removeClass('oculta').addClass('visible').addClass('solved');
		var isCorrect = $('#'+idSol).hasClass('solved') ? true : false;
		sendSuccess(isCorrect);
	}
	setTimeout(function(){
		if(NUMBER_DIFFS==$('.solImg.solved').length){showFinishGame();}
	},3000);
}
function showFinishGame(){
	changePhase(3);
	solution=new Array();
	$('#finishGame').removeClass('oculta').addClass('visible');
	$('#sombreado').removeClass('oculta').addClass('visible');
	$('#hintButton').removeClass("visible").addClass("oculta");
}
function selectNumberOfCards(level){
	switch(level){
		case 1:
			NUMBER_PAIRS=2;
			NUMBER_COLS=4;
			break;
		case 2:
			NUMBER_PAIRS=4;
			NUMBER_COLS=4;
			break;
		case 3:
			NUMBER_PAIRS=6;
			NUMBER_COLS=4;
			break;
		default:
			NUMBER_PAIRS=4;
			NUMBER_COLS=4;
	}
	hideInitGame();
}
function hideInitGame(){
	$('#initGame').removeClass('visible').addClass('oculta');
	$('#sombreado').removeClass('visible').addClass('oculta');
	startPolling();
	loadGame();
}
function alreadyPickedUp(e,v){
	return v.indexOf(e)!==-1;
}
function disorderCards(cards){
	var i, temp, L= cards.length, A= cards.concat();
	while(--L){
		i= Math.floor(Math.random()*L);
		temp= A[i];
		A[i]= A[L];
		A[L]= temp;
	}
	return A;
}
function getNumDifferenceOfImage(){
	switch(idImg){
		case 1:
			return 3;
		case 2:
			return 2;
		default:
			return 0;
	}
}
function loadGame(){
	var game_pieces=new Array();
	if(id_game=="memGame"){
		var number = ["as","2","3","4","5","6","7","sota","caballo","rey"];
		var suit = ["Oros","Bastos","Copas","Espadas"];
		var cards = new Array();
		for(var i=0; i<NUMBER_PAIRS; i++){
			var numberPosition = Math.floor(Math.random() * number.length);
			var suitPosition = Math.floor(Math.random() * suit.length);
			var card = number[numberPosition]+suit[suitPosition];
			while(alreadyPickedUp(card,cards)){
				numberPosition = Math.floor(Math.random() * number.length);
				suitPosition = Math.floor(Math.random() * suit.length);
				card = number[numberPosition]+suit[suitPosition];
			}
			cards.push(card);		
		}
		game_pieces=disorderCards(cards.concat(cards));		
	} else if(id_game=="diffGame"){
		//We'll show a different image each time we play
		idImg = Math.floor(Math.random()*NUMBER_DIFFERENT_IMAGES+1);
		game_pieces["idImg"]=idImg;
		var image_path = "img/"+id_game+"/differences_"+idImg;
		game_pieces["init"]=image_path+".png";
		NUMBER_DIFFS=getNumDifferenceOfImage();
		for(var i=0; i<NUMBER_DIFFS; i++)
			game_pieces["solution_"+i]=image_path+"_solution_"+i+".png";
	}
	findSolution(game_pieces);
	buildContentGame(game_pieces);
	changePhase(2); //Start playing
}

function getColWidth(){
	switch(NUMBER_COLS){
		case 5: case 6:
			return "col-lg-2"; //16.67%
		case 4:
			return "col-lg-3"; //25%
		case 3:
			return "col-lg-4"; //33%
		default:
			return "col-lg-1"; //8.33%
	}
}
function buildContentGame(game_pieces){

	var htmlToInsert = "";
	if(id_game=="memGame"){
		var numFilas = game_pieces.length/NUMBER_COLS;
		var ultimo=0;
		var colWidth = getColWidth();
		for(var fila=0; fila<numFilas; fila++){
			htmlToInsert += '<div class="cardsRow">';
			for(var i=ultimo+1; i<ultimo+NUMBER_COLS+1; i++){
				htmlToInsert += '<div id="card'+i+'" class="card '+colWidth+'" onclick="toggleCard('+"'card"+i+"'"+');return false;">'
					 + '<img class="backCard visible" src="img/back'+i+'.png">'
					 + '<img class="frontCard oculta"  src="img/spanishDockOfCards/'+game_pieces[i-1]+'.jpg">'
					 + '</div>';
			}
			ultimo=i-1;
			htmlToInsert += '</div>';
		}	
	}else if(id_game=="diffGame"){
		htmlToInsert += '<div class="diffRow">';
		htmlToInsert += '<img class="visible" src="'+game_pieces["init"]+'">';
		for(var i=0; i<NUMBER_DIFFS; i++)
			htmlToInsert += '<img class="oculta solImg" id="image'+idImg+'_diff'+i+'" src="'+game_pieces["solution_"+i]+'">';
		htmlToInsert += '</div>';
		startPolling();
	}
	$('#content').html(htmlToInsert);
	if(id_game=="memGame")
		$('#hintButton').removeClass("oculta").addClass("visible");
}
function reloadGame(){
	window.location.href=window.location.href;
}

function findSolution(game_pieces){
	if(id_game=="memGame"){
		var j=0; var numPairsFound=0; 
		while(numPairsFound!=NUMBER_PAIRS && j<game_pieces.length){
			for(var i=0; i<game_pieces.length; i++){
				if(i!==j && game_pieces[j]==game_pieces[i] && solution[j+1]==undefined && solution[i+1]==undefined){
					numPairsFound++;
					solution[j+1]=i+1;
				}
			}
			j++;
		}
	}
}

function updateSolution(possiblePair){
	if(id_game=="memGame"){
		var first = parseInt($(possiblePair[0]).selector.substring(5,6));
		var second = parseInt($(possiblePair[1]).selector.substring(5,6));
		if(solution[first]!==undefined) solution[first]=undefined;
		if(solution[second]!==undefined) solution[second]=undefined;
	}
}

function getHint(){
	if(id_game=="memGame"){
		var found=false; var i=0;
		var hintPair = ["",""];
		while(!found && i<2*NUMBER_PAIRS){
			if(solution[i]!==undefined){
				found=true;
				hintPair=[i,solution[i]];
			}else{
				i++;
			}
		}
		for(var i=0; i<hintPair.length; i++){
			$("#card"+hintPair[i]+' .backCard').addClass('hint');
		}
		setTimeout(function(){
			for(var i=0; i<hintPair.length; i++){
				$("#card"+hintPair[i]+' .backCard').removeClass('hint');
			}
		},3000);	
	}
}
