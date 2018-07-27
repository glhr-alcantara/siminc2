<?php
// Exclui ação
if ( ($_REQUEST['action'] == 'delete') && !empty($_REQUEST['id'])){
	
    $acaid = $_REQUEST['id'];
    
	//Verifica a existência de vinculos
	$sql = "SELECT SUM(total) as total
			FROM(
				SELECT count(0) as total FROM painel.acaosecretaria WHERE acaid = {$acaid}
				UNION ALL
				SELECT count(0) as total FROM painel.indicador WHERE indstatus = 'A' AND acaid = {$acaid}
				UNION ALL
				SELECT count(0) as total FROM pde.ae_acaoxdesafio WHERE acaid = {$acaid}
				UNION ALL
				SELECT count(0) as total FROM pde.ae_acaoxmetapne WHERE acaid = {$acaid}
				UNION ALL
				SELECT count(0) as total FROM pde.ae_acaoxmetappa WHERE acaid = {$acaid}
				UNION ALL
				SELECT count(0) as total FROM monitora.pi_acaoestrategicasubacao WHERE acaid = {$acaid}
				UNION ALL
				SELECT count(0) as total FROM planacomorc.vinculacaoacaoestrategica WHERE acaid = {$acaid}
			) AS foo";
	$valor = $db->pegaum($sql);
	
	if ($valor > 0){
		echo "<script>
				alert('A ação estratégica não pode ser excluída, pois possui vínculos. Remova-os e tente novamente!');		  
				</script>";
	}
	else {
            // Exclui ação
            $sql = "UPDATE painel.acao set acastatus='I' WHERE acaid = {$acaid}";
            $db->executar($sql);
            $db->commit();
	
	echo "<script>
                alert('Ação estratégica excluída com sucesso!');
              </script>";
	}
}

include  APPRAIZ."includes/cabecalho.inc";
include_once APPRAIZ. "includes/combo_popup_class.php";
include_once APPRAIZ. "includes/campo_texto_class.php";
echo "<br>";

if(isset($_SESSION['acaid'])){
    unset($_SESSION['acaid']);
}

$menu = array();
$menu[0] = array("descricao" => "Pesquisa de ações estratégicas", "link"=> "/painel/painel.php?modulo=sistema/tabelaapoio/cadastroacoes/listar_acao&acao=A");
$menu[1] = array("descricao" => "Cadastro da ação estratégica", "link"=> "/painel/painel.php?modulo=sistema/tabelaapoio/cadastroacoes/formulario_acao&acao=A");
echo montarAbasArray($menu, "/painel/painel.php?modulo=sistema/tabelaapoio/cadastroacoes/listar_acao&acao=A");

monta_titulo( $titulo_modulo , '');
?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario" action="/painel/painel.php?modulo=sistema/tabelaapoio/cadastroacoes/listar_acao&acao=A">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td align='right' class="SubTituloDireita">Ação estratégica:</td>
    <td><?php
        $campo_texto = new campo_texto();
        $campo_texto->var = 'sbacao';
        $campo_texto->obrig='N';
        $campo_texto->habil='S';
        $campo_texto->size=100;
        $campo_texto->max=255;
        $campo_texto->value = $_REQUEST['sbacao']?$_REQUEST['sbacao']:'';
//    	echo campo_texto('sbacao','N','S','',100,255,'',''); 
    	echo $campo_texto->gerarComponente();
//    	echo "<input type='text' name='sbatitulo'>";
    	?>
    </td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">Unidade:</td>
    <td>
        <?php
            $sql = "select secid as codigo, secdsc as descricao from painel.secretaria where secstatus = 'A' ORDER BY 2";
            $combo_popup = new combo_popup();
            $combo_popup->nome = 'secid';
            $combo_popup->sql = $sql;
            $combo_popup->titulo = 'Selecione a(s) Unidade(s)';
            $combo_popup->tamanho_janela = '360x460';
            $combo_popup->size = 3;
            if (!empty($_REQUEST['secid'][0])){
                $sql_preenchidos = "select secid as codigo, secdsc as descricao from painel.secretaria where secstatus = 'A' and secid in (".implode(', ', $_REQUEST['secid']).") ORDER BY 2";
                $aPreenchidos = $db->carregar($sql_preenchidos);
                $combo_popup->value = $aPreenchidos;
            }
            $combo_popup->gerarComponente();
        ?>
    </td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">Eixo:</td>
    <td>
        <?php
            $sql = "select exoid as codigo, exodsc as descricao from painel.eixo ORDER BY 2";
            $combo_popup = new combo_popup();
            $combo_popup->nome = 'exoid';
            $combo_popup->sql = $sql;
            $combo_popup->titulo = 'Selecione o(s) Eixo(s)';
            $combo_popup->tamanho_janela = '360x460';
            $combo_popup->size = 3;
            if (!empty($_REQUEST['exoid'][0])){
                $sql_preenchidos = "select exoid as codigo, exodsc as descricao from painel.eixo where exoid in (".implode(', ', $_REQUEST['exoid']).") ORDER BY 2";
                $aPreenchidos = $db->carregar($sql_preenchidos);
                $combo_popup->value = $aPreenchidos;
            }
            $combo_popup->gerarComponente();
        ?>
    </td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">Diretriz:</td>
    <td>
        <?php
            $sql = "select dirid as codigo, dirdsc as descricao from painel.diretriz where dirstatus = 'A' ORDER BY 2";
            $combo_popup = new combo_popup();
            $combo_popup->nome = 'dirid';
            $combo_popup->sql = $sql;
            $combo_popup->titulo = 'Selecione o(s) Eixo(s)';
            $combo_popup->tamanho_janela = '360x460';
            $combo_popup->size = 3;
            if (!empty($_REQUEST['dirid'][0])){
                $sql_preenchidos = "select dirid as codigo, dirdsc as descricao from painel.diretriz where dirstatus = 'A' and dirid in (".implode(', ', $_REQUEST['dirid']).") ORDER BY 2";
                $aPreenchidos = $db->carregar($sql_preenchidos);
                $combo_popup->value = $aPreenchidos;
            }
            $combo_popup->gerarComponente();
        ?>
    </td>
</tr>
<tr bgcolor="#cccccc" class="notprint">
    <td></td>
        <td>
            <input type="button" class="botao" name="bta" value="Pesquisar" onclick="javascript:submeter();">
        </td>
    </tr>
</table>
</form>
<?php
//ver($_REQUEST,d);
$where = " WHERE ac.acastatus = 'A'";
if(!empty($_REQUEST['sbacao'])){
    $where .=" and upper(ac.acadsc) like upper('%".$_REQUEST['sbacao']."%') ";
}
if (!empty($_REQUEST['secid'][0])){
    $whereSec =" and asec.acaid in (select acaid from painel.acaosecretaria where secid in (".implode(', ', $_REQUEST['secid']).")) "; 
}
if (!empty($_REQUEST['exoid'][0])){
    $where .=" and ind.exoid in (".implode(', ', $_REQUEST['exoid']).")";
}
if (!empty($_REQUEST['dirid'][0])){
    $where .=" and ac.dirid in (".implode(', ', $_REQUEST['dirid']).")";
}
	$sql = "SELECT '<center class=\"notprint\">
						<a style=\"cursor:pointer;\" onclick=\"alteraracao(\''||ac.acaid||'\');\">
							<img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
						</a>
						<a  style=\"cursor:pointer;\" onclick=\"removeracao(\''||ac.acaid||'\');\">
							<img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
						</a>
					</center>' as acao, 
				ac.acadsc,
                                array_to_string(array_agg(s.secdsc), ' / ')
				FROM painel.acao ac 
                                left join painel.acaosecretaria asec
                                  on ac.acaid = asec.acaid
                                left join painel.secretaria s
                                  on asec.secid = s.secid
                                left join painel.indicador ind
                                  on ac.acaid = ind.acaid
				$where
                                $whereSec
                                group by ac.acaid, ac.acadsc
				ORDER BY ac.acadsc
				";
 
	$cabecalho = array("","Título", "Gestores");
//                         $sql,$cabecalho="",$perpage,$pages,$soma,$alinha,$valormonetario="S",$nomeformulario="",$celWidth=""
	$db->monta_lista($sql,$cabecalho,100,5,'N','','S','',array('5%','55%','60%'),$par2);
//}
?>
<script type="text/javascript">
    function submeter(){
        prepara_formulario();
        document.formulario.submit();	
    }

    function removeracao(acaid){
            var conf = confirm("Você realmente deseja excluir esta ação estratégica?");	
            if(conf) {
                    window.location.href = window.location.href + "&action=delete&id="+acaid;	
            }
    }
    function alteraracao(acaid){
        var janela = window.location.href='/painel/painel.php?modulo=sistema/tabelaapoio/cadastroacoes/formulario_acao&acao=A&id='+acaid;
    }
</script>