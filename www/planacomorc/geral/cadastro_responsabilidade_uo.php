<?php
// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
require (APPRAIZ . 'www/planacomorc/_constantes.php');
$db = new cls_banco();

function gravarResponsabilidadeAcao($dados) {
    global $db;
    $sql = "UPDATE planacomorc.usuarioresponsabilidade SET rpustatus='I' WHERE usucpf='".$dados['usucpf']."' AND pflcod='".$dados['pflcod']."'";
    $db->executar($sql);

    if ($dados['usuacaresp']) {
        foreach($dados['usuacaresp'] as $unicod) {

            $sql = <<<DML
INSERT INTO planacomorc.usuarioresponsabilidade(pflcod, usucpf, rpustatus, rpudata_inc, unicod)
  VALUES ('{$dados['pflcod']}', '{$dados['usucpf']}', 'A', NOW(), '{$unicod}')
DML;
            $db->executar($sql);
        }
    }

    $db->commit();

    echo "<script language=\"javascript\">
                alert(\"Opera��o realizada com sucesso!\");
                opener.location.reload();
                self.close();
          </script>";
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
?>
<html>
<head>
<meta http-equiv="Pragma" content="no-cache">
<title>Defini��o de responsabilidades - Unidade Or�ament�ria</title>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script src="../js/planacomorc.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='/includes/listagem.css'>
<style type="text/css">
.tabela{width:100%}
</style>
</head>
<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff" onload="self.focus()">
<script>
function marcarAcao(obj) {
    if(obj.checked) {
        if (!jQuery('#usuacaresp option[value='+obj.value+']')[0]) {
            jQuery("#usuacaresp").append('<option value='+obj.value+'>'+obj.parentNode.parentNode.cells[1].innerHTML+'</option>');
        }
    } else {
        jQuery('#usuacaresp option[value='+obj.value+']').remove();
    }
}
</script>
<div style="overflow:auto;width:496px;height:350px;border:2px solid #ececec;background-color:white">
<?php
monta_titulo('Defini��o de responsabilidades - Unidade Or�ament�ria', '');

// -- � feita uma verifica��o no SQL para saber se aquele ungcod j� foi escolhido previamente
// -- com base nisso, � adicionado o atributo checked ao combo do unicod selecionado previamente.
$unidadesObrigatorias = UNIDADES_OBRIGATORIAS;
$sql = "
SELECT '<input type=\"checkbox\" name=\"unicod\" id=\"chk_' || uni.unicod || '\" value=\"' || uni.unicod || '\" '
           || 'onclick=\"marcarAcao(this)\"'
           || case WHEN (SELECT count(urp.rpuid) FROM planacomorc.usuarioresponsabilidade urp WHERE urp.unicod = uni.unicod AND urp.usucpf = '{$usucpf}' AND urp.pflcod = '{$pflcod}' AND rpustatus = 'A') > 0 THEN ' checked' ELSE '' END || '>' AS unicod,
       uni.unicod || ' - ' || uni.unidsc AS descricao
  FROM public.unidade uni
 --   LEFT JOIN planacomorc.usuarioresponsabilidade urp
 --     on urp.unicod = uni.unicod AND urp.usucpf = '{$usucpf}' AND urp.pflcod = '{$pflcod}'
  WHERE uni.unistatus = 'A'
    --AND uni.unicod IN ($unidadesObrigatorias)
    AND orgcod = '". CODIGO_ORGAO_SISTEMA. "'     
  ORDER BY uni.unicod::integer
";

$cabecalho = array('', 'UO - Descri��o');
$db->monta_lista_simples($sql, $cabecalho, 500, 5, 'N', '100%', 'N');
?>
</div>
<form name="formassocia" style="margin:0px;" method="POST">
<input type="hidden" name="usucpf" value="<?=$usucpf?>">
<input type="hidden" name="pflcod" value="<?=$pflcod?>">
<input type="hidden" name="requisicao" value="gravarResponsabilidadeAcao">
<select multiple size="8" name="usuacaresp[]" id="usuacaresp" style="width:500px;" class="CampoEstilo">
<?
$sql = <<<DML
SELECT uni.unicod AS codigo,
       uni.unicod || ' - ' || uni.unidsc AS descricao
  FROM planacomorc.usuarioresponsabilidade ur
    INNER JOIN public.unidade uni USING(unicod)
  WHERE ur.usucpf = '{$usucpf}'
    AND ur.pflcod = '{$pflcod}'
    AND ur.rpustatus = 'A'
    AND uni.unicod IN ($unidadesObrigatorias)
DML;
$usuarioresponsabilidade = $db->carregar($sql);

if($usuarioresponsabilidade[0]) {
	foreach($usuarioresponsabilidade as $ur) {
		echo '<option value="'.$ur['codigo'].'">'.$ur['descricao'].'</option>';
	}
}
?>
</select>
</form>
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
	<tr bgcolor="#c0c0c0">
		<td align="right" style="padding:3px;" colspan="3">
			<input type="Button" name="ok" value="OK"
                   onclick="selectAllOptions(document.getElementById('usuacaresp'));document.formassocia.submit();"
                   id="ok">
		</td>
	</tr>
</table>
