<?php

// verifica se ocorre algum evento
if(isset($_REQUEST['action']) && $_POST && ($_REQUEST['action'] != '') ){
	switch($_REQUEST['action']) {
            
            // atualizar os dados da a��o
            case "save":
            $sql = "DELETE FROM pde.ae_objetivoestrategicoxiniciativa WHERE obeid = {$_SESSION['id']}";
            $db->executar($sql);
            
            if(is_array($_POST['ids'])) {
                foreach($_POST['ids'] as $id ) {
                    $sql = "INSERT INTO pde.ae_objetivoestrategicoxiniciativa
                                            (obeid, iniid) 
                           VALUES ({$_SESSION['id']},$id)
                       ";
                    
                    $db->pegaUm($sql);
                }
            }		

            $db->commit();

            echo "<script>
                    alert('Salvo com sucesso!');
                </script>";
            break;
	}
}

include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( $titulo_modulo , '');

if($_SESSION['id']){
    $id = $_SESSION['id'];
} else {
    $id = '';
}
if($id) {
    $sql = "SELECT mppa.iniid AS codigo, mppa.ininome AS descricao
            FROM pde.ae_objetivoestrategicoxiniciativa ax
            LEFT JOIN pde.ae_iniciativa mppa ON mppa.iniid = ax.iniid
            WHERE ax.obeid = $id
			ORDER BY descricao";
    $itens = $db->carregar($sql);
}

?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario">
<input type="hidden" name="action" id="action" value="save">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

<tr>
	<td class="SubTituloDireita" valign="top">Iniciativas vinculadas:</td>
    <td >
        <table cellpadding="0" border="0" width="98%"  id="orcamento"  style="BORDER-RIGHT: #C9C9C9 1px solid; BORDER-TOP: #C9C9C9 1px solid; BORDER-LEFT: #C9C9C9 1px solid; BORDER-BOTTOM: #C9C9C9 1px solid;">
		<? 
		if($itens[0]) {
			$valortotal = 0;
			$cor = 0;
			foreach($itens as $item) { 
		?>
        <tr style="height:30px;<? echo (($cor%2)?"":"background-color:#DCDCDC;"); ?>" id="id_<? echo $item['codigo']; ?>">
			<td align="left">
				
				<? 
				echo "<input type='checkbox' checked id='check' onclick='resultado({$item['codigo']});'>".$item['descricao']; ?>
				<input type='hidden' name='ids[<? echo $item['codigo']; ?>]' value='<? echo $item['codigo']; ?>'>
			</td>
		</tr>
		<? 
				$cor++;
			}
		} 
		?>
        <tr>
            <td align="right" colspan="6"><input type="button" onclick="abrir_lista();" id="btn_selecionar_acaptres" value="Vincular Iniciativa"></td>
        </tr>
        </table>
    </td>
</tr>

<tr bgcolor="#cccccc">
	      <td></td>
	  	  <td>
	  	  <input type="button" class="botao" name="btg" value="Gravar" onclick="submeter();">
	      </tr>
</table>
</form>
<script type="text/javascript">
function submeter() {
	document.formulario.submit();
}

function abrir_lista() {
    var janela = window.open( 'estrategico.php?modulo=sistema/alinhamento_estrategico/lista_vincular&acao=A&lista=iniciativa', 'blank', 'height=600,width=500,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );
    janela.focus();
}

function resultado(obeid){
    var tabela = document.getElementById('orcamento');
    tabela.deleteRow(window.document.getElementById('id_'+obeid).rowIndex);
}

</script>