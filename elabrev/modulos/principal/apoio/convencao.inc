<script type="text/javascript">
	var codigos = new Array(); 
</script>
<?php
include APPRAIZ . "includes/cabecalho.inc";
/*******************************
* 	VARIAVEIS
*	Recebe dados do formulario 
*******************************/
$pstNum			= count($_POST);
$pstIncluir		= $_POST["inserir"];
$pstExcluir		= $_POST["excluir"];
$pstEditar		= $_POST["editar"];
$pstCodigo		= $_POST["codigo"];
$pstDescricao	= $_POST["descricao"];

/*******************************
* 	A��O
*	insere, edita e exclui da 
*   tabela de conven��o 
*******************************/
if($pstNum && $pstIncluir){
	$sql="INSERT INTO financeiro.tipoconvencao (tpccod, tpcdsc) VALUES ('".$pstCodigo."','".$pstDescricao."')";
	$db->executar( $sql );
	$alerta = "Conven��o incluida com sucesso.";
}

if($pstNum && $pstEditar){
	$sql="UPDATE financeiro.tipoconvencao 
		  SET 	tpccod = '".$pstCodigo."', 
		  		tpcdsc = '".$pstDescricao."'
		  WHERE tpcid = ".$pstEditar.";";
	$db->executar( $sql );
	$alerta = "Conven��o alterada com sucesso.";
}

if($pstNum && $pstExcluir){
	$sql="DELETE FROM financeiro.tipoconvencao WHERE tpcid=".$pstExcluir;
	$db->executar( $sql );
	$alerta = "Conven��o excluida com sucesso.";
}

if($pstNum){
	echo "<script> 
			alert( '".$alerta."');
    	  </script>";
	$db->commit();
}

/*******************************
* 	MONTA LISTA
*	Recupera a lista de conven��o 
*   e monta a lista.
*******************************/
$sql= "	SELECT
			'<a href=\"#\"><img onclick=\"editar('||tpcid||',\''||tpccod||'\',\''||tpcdsc||'\')\" src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a> '
			' &nbsp;<a style=\"margin: 0 0 0 5px;\" href=\"\"> <img onclick=\"excluir('||tpcid||')\" src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a>'as acoes,
			tpccod, 
			tpcdsc 
		FROM financeiro.tipoconvencao ORDER BY tpccod asc";
$cabecalho = array ("A��es ","C�digo", "Descri��o");

$sqlvalidacao= "SELECT tpccod FROM financeiro.tipoconvencao";
$codexiste = $db->carregar($sqlvalidacao);
if($codexiste != NULL){
foreach($codexiste as $chave=>$codigo){
?>
<script type="text/javascript">
	codigos[<?=$chave ?>] = <?=$codigo['tpccod']?>; 
</script>
<?	
}
}	
?>
<table width="95%" border="0" cellspacing="0" cellpadding="2" align="center" bgcolor="#f7f7f7" style="border-top: 1px solid #c0c0c0;">
    <form name="formulario" method="post">
	    <input type='hidden' name='inserir' value=0>
	    <input type='hidden' name='excluir' value=0>
	    <input type='hidden' name='editar' value=0>
		<tr>
		<th colspan="2"><h1>Tabela de conven��o</h1></th>
		</tr>
		<tr>
		<td class="SubTituloDireita">C�digo:</td>
		<td><input style="width:320px;" type="text" name="codigo" id="codigo" /></td>
		</tr>
		<tr>
		<td class="SubTituloDireita">Descri��o:</td>
		<td><input style="width:320px;" type="text" name="descricao" id="descricao" /></td>
		</tr>
		<tr>
		<th colspan="2"> 
			<input type="button" name="incluir" value="incluir" onclick="inserirdados('novo');"/>
			<input style="display:none" type="button" name="alterar" value="alterar"  onclick="inserirdados('alterar');" />
			<input type="button" name="voltar" value="voltar"  onclick="voltarpag();"  />
		</th>
		</tr>
	</form>
</table>
<?
/*******************************
* 	MONTA LISTA
*******************************/
$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', 'left', '' );
?>
<script type="text/javascript">
	/*Submete os dados.*/
	var codigoinicial ="";
	function inserirdados(valor) {
		descricao 	= document.getElementById('descricao').value;
			cod 	= document.getElementById('codigo').value;
		if(descricao == "" ){
			alert("O campo descri��o � obrigatorio");
			return false;
		}
		if(cod == "" ){
			alert("O campo c�digo � obrigatorio");
			return false;
		}
		for(i=0; i<codigos.length; i++){
			if(codigos[i]== cod && codigoinicial != cod ){
				alert("Este c�digo j� est� cadastrado para outra conven��o.");
				return false;
			}
		}
		if(valor == 'novo'){
			document.formulario.inserir.value = 1;
		}
		document.formulario.submit();
	}
	
	/*Exclui a conven��o pelo c�digo*/
	function excluir(cod) { 
		if( window.confirm( "Confirma a exclus�o do �tem "+ cod + " de conven��o?") ){
			document.formulario.excluir.value = cod;
			document.formulario.submit();
	    }else document.formulario.excluir.value = 0;
	}
	
	/*Edita a conven��o pelo codigo*/
	function editar(id, cod, desc){
		document.formulario.editar.value = id;
		document.formulario.descricao.value = desc;
		document.formulario.incluir.style.display="none";
		document.formulario.codigo.value = cod;
		document.formulario.alterar.style.display="inline";
		codigoinicial=document.getElementById('codigo').value;
	}
	
	/*BTN Voltar */
	function voltarpag(){
		if(document.formulario.incluir.style.display == "none"){
			document.formulario.incluir.style.display="inline";
			document.formulario.alterar.style.display="none";
			document.formulario.descricao.value = "";
			document.formulario.codigo.value = "";
			document.formulario.editar.value = 0;
		}else{
			history.back();
		}
	}
</script>