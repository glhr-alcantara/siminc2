<?php
/**
 * Sistema de cria��o e gest�o de propostas or�ament�rias.
 * $Id: inicio.inc 103881 2015-10-20 19:46:55Z werteralmeida $
 */
/*
 * Componetnes para a cria��o da p�gina In�cio
 */
include APPRAIZ . "includes/dashboardbootstrap.inc";

/**
 * Recupera um array com os menus e seus itens de menus de maneira recursivamente.
 *
 * @global object $db - class
 * @param integer $sisId
 * @param string $cpf
 * @return array $menus
 */
function carregarMenusBox($sisId, $menuPaiId, $cpf) {
    global $db;

    // Carregando o menu e itens de menu deste sistema e dos usuarios especificos.
    $lista_cpf = "'" . $cpf . "'";

    $sql = <<<DML
SELECT DISTINCT mnu.mnucod,
                mnu.mnuid,
                mnu.mnuidpai,
                mnu.mnudsc,
                mnu.mnustatus,
                mnu.mnulink,
                mnu.mnutipo,
                mnu.mnustile,
                mnu.mnuhtml,
                mnu.mnusnsubmenu,
                mnu.mnutransacao,
                mnu.mnushow,
                mnu.abacod
  FROM seguranca.menu mnu,
       seguranca.perfilmenu pmn,
       seguranca.perfil pfl,
       seguranca.perfilusuario pfu
  WHERE mnu.mnuid = pmn.mnuid
    AND pmn.pflcod = pfl.pflcod
    AND pfl.pflcod = pfu.pflcod
    AND pfu.usucpf IN({$lista_cpf})
    AND (mnu.mnutipo = 1 OR mnu.mnuidpai IS NOT NULL)
    AND mnu.mnushow = 't'
    AND mnu.mnustatus = 'A'
    AND mnu.sisid={$sisId}
    AND mnu.mnuidpai = (SELECT mnuid
                          FROM seguranca.menu
                          WHERE mnucod = {$menuPaiId}
                            AND sisid = {$sisId})
  ORDER BY mnu.mnucod,
           mnu.mnuid,
           mnu.mnuidpai,
           mnu.mnudsc
DML;

    $menusBd = @$db->carregar($sql);
    return $menusBd;
}

/**
 * Cabecalho do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<link href="/includes/spo.css" rel="stylesheet" media="screen">
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script type="text/javascript" lang="javascript">
    $(document).ready(function () {
        inicio();
    });
    /**
     * Abre um arquivo cadastrado no sistema de comunicados.
     * @param {int} arqid O ID do arquivo no sistema.
     */
    function abrirArquivo(arqid) {
        var uri = window.location.href;
        uri = uri.replace(/\?.+/g, '?modulo=principal/documento/visualizar&acao=A&download=S&arqid=' + arqid);
        window.location.href = uri;
    }
</script>

<table width="95%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
    <tr align="center">
        <td align="center">
            <div class="divCap" style="background-color: #D2691E">
                <span class="tituloCaixa">Iniciar o Exerc�cio de <?php echo $_SESSION['exercicio']; ?> (MEC)</span>
                <?php
                $params = array();
                $params['texto'] = 'SPO - Altera��es Or�ament�rias';
                $params['tipo'] = 'central';
                $params['url'] = '';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'SPO - Planejamento e Acompanhamento Or�ament�rio';
                $params['tipo'] = 'central';
                $params['url'] = '';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'SPO - Proposta Or�ament�ria';
                $params['tipo'] = 'central';
                $params['url'] = '';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'SPO - Receita Or�ament�ria';
                $params['tipo'] = 'central';
                $params['url'] = '';
                montaBotaoInicio($params);
                ?>
            </div>
            <div class="divCap" style="background-color: #8B7958">
                <span class="tituloCaixa">Per�odos de <?php echo $_SESSION['exercicio']; ?> (MEC)</span>
                <?php
                $params = array();
                $params['texto'] = 'Per�odo de Acompanhamento de A��es';
                $params['tipo'] = 'calendario';
                $params['url'] = 'spo.php?modulo=principal/periodo/periodo&acao=A&tipo=acao';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Per�odo de Acompanhamento de Suba��es';
                $params['tipo'] = 'calendario';
                $params['url'] = 'spo.php?modulo=principal/periodo/periodo&acao=A&tipo=subacao';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Per�odo de Acompanhamento de Relat�rio do TCU';
                $params['tipo'] = 'calendario';
                $params['url'] = 'spo.php?modulo=principal/periodo/periodo&acao=A&tipo=relatorio';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Per�odo de Altera��o de Receita Or�ament�ria';
                $params['tipo'] = 'calendario';
                $params['url'] = 'spo.php?modulo=principal/periodo/periodo&acao=A&tipo=receita';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Momento de Altera��es de Cr�dito';
                $params['tipo'] = 'calendario';
                $params['url'] = 'spo.php?modulo=principal/periodo/periodo&acao=A&tipo=alteracao';
                montaBotaoInicio($params);

                $params = array();
                $ploa = $_SESSION['exercicio'] + 1;
                $params['texto'] = "Per�odo de Preenchimento da Proposta (PLOA {$ploa})";
                $params['tipo'] = 'calendario';
                $params['url'] = 'spo.php?modulo=principal/periodo/periodo&acao=A&tipo=ploa';
                montaBotaoInicio($params);
                ?>
            </div>
            <div class="divCap" style="background-color: #ccab1f">
                <span class="tituloCaixa">Snapshots (MEC)</span>
                <?php
                $params = array();
                $params['texto'] = 'Acompanhamento de A��es';
                $params['tipo'] = 'snapshot';
                $params['url'] = 'spo.php?modulo=principal/snapshot/acompanhamentoacao&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Acompanhamento de Suba��es';
                $params['tipo'] = 'snapshot';
                $params['url'] = 'spo.php?modulo=principal/snapshot/acompanhamentoSubacao&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $ploa = $_SESSION['exercicio'] + 1;
                $params['texto'] = "Proposta Or�ament�ria (PLOA {$ploa})";
                $params['tipo'] = 'snapshot';
                $params['url'] = 'spo.php?modulo=principal/processamento/cargaploa&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Momento de Altera��es Or�ament�rias';
                $params['tipo'] = 'snapshot';
                $params['url'] = 'spo.php?modulo=principal/snapshots/momentoalteracaocredito&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Altera��o de Receita Or�ament�ria';
                $params['tipo'] = 'snapshot';
                $params['url'] = '';
                montaBotaoInicio($params);
                ?>
            </div>
            <div class="divCap" style="background-color: #B50000">
                <span class="tituloCaixa">Processamentos (MEC)</span>
<?php
$params = array();
$params['texto'] = 'Processar dados carga da LOA';
$params['tipo'] = 'processar';
$params['url'] = 'spo.php?modulo=principal/processamento/cargaloa&acao=A';
montaBotaoInicio($params);

$params = array();
$params['texto'] = 'Configura��o da PLOA';
$params['tipo'] = 'tabelaapoio';
$params['url'] = 'spo.php?modulo=principal/ploa/configuracao&acao=A';
montaBotaoInicio($params);

$params = array();
$params['texto'] = 'Processar dados da Receita Or�ament�ria';
$params['tipo'] = 'processar';
$params['url'] = '';
montaBotaoInicio($params);

$params = array();
$params['texto'] = 'Relat�rio de Gest�o (TCU/CGU) / Importar preenchimento do Acompanhamento de A��es';
$params['tipo'] = 'processar';
$params['url'] = '';
montaBotaoInicio($params);

$params = array();
$params['texto'] = 'Processar dados de Altera��es Or�ament�rias';
$params['tipo'] = 'processar';
$params['url'] = '';
montaBotaoInicio($params);
?>
            </div>
            <br style="clear: both"/>
            <div id="divLiberacaoFinanceira" class="divGraf">
                <span class="tituloCaixa">SIOP (SOF)</span>
                <?php
                $params = array();
                $params['texto'] = 'Qualitativo';
                $params['tipo'] = 'baixarws';
                $params['target'] = '#qualitativo';
                $params['toggle'] = 'modal';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Quantitativo';
                $params['tipo'] = 'baixarws';
                $params['target'] = '#quantitativo';
                $params['toggle'] = 'modal';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Altera��es Or�ament�rias';
                $params['tipo'] = 'baixarws';
                $params['url'] = '';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Receita Or�ament�ria';
                $params['tipo'] = 'baixarws';
                $params['url'] = 'spo.php?modulo=integracao/sof/receita/baseexterna&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Carga da Execu��o Or�ament�ria';
                $params['tipo'] = 'baixarws';
                $params['url'] = 'spo.php?modulo=integracao/sof/cargaexecucaoorcamentaria/acompanhamento&acao=A';
                montaBotaoInicio($params);
                ?>
                <span class="subTituloCaixa">Relat�rios</span>
                <?php
                $params = array();
                $params['texto'] = 'Listar dados do Qualitativo';
                $params['tipo'] = 'listar';
                $params['url'] = 'spo.php?modulo=relatorio/sof/listadadosqualitativo&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Listar dados do Quantitativo';
                $params['tipo'] = 'listar';
                $params['url'] = 'spo.php?modulo=relatorio/sof/listadadosquantitativo&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Listar dados das Altera��es Or�ament�rias';
                $params['tipo'] = 'listar';
                $params['url'] = '';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Listar dados da Receita Or�ament�ria';
                $params['tipo'] = 'listar';
                $params['url'] = '';
                montaBotaoInicio($params);
                ?>
                <span class="subTituloCaixa">Logs</span>
                <?php
                $params = array();
                $params['texto'] = 'LOG dos WebServices SOF';
                $params['tipo'] = 'log';
                $params['url'] = 'spo.php?modulo=relatorio/logs/listar&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'LOG da Altera��es Or�ament�rias';
                $params['tipo'] = 'log';
                $params['url'] = 'spo.php?modulo=relatorio/logs/alteracaoorcamentaria&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'LOG da Receita Or�ament�ria';
                $params['tipo'] = 'log';
                $params['url'] = 'spo.php?modulo=relatorio/logs/receitaorcamentaria&acao=A';
                montaBotaoInicio($params);

                montaBotaoInicio(array(
                    'texto' => 'LOG da Proposta Or�ament�ria',
                    'tipo' => 'log',
                    'url' => 'spo.php?modulo=relatorio/logs/propostaorcamentaria&acao=A'
                ));
                ?>
            </div>

            <div class="divCap" id="divEspelho" data-request="">
                <span class="tituloCaixa">SIAFI (SERPRO)</span>
<?php
$params = array();
$params['texto'] = 'LOG da Programa��o Financeira';
$params['tipo'] = 'log';
$params['url'] = 'spo.php?modulo=relatorio/logs/programacaofinanceira&acao=A';
montaBotaoInicio($params);
?>
            </div>

            <div class="divCap" style="background-color: darksalmon">
                <span class="tituloCaixa">SIGEF/DOCUMENTA (FNDE)</span>
<?php
$params = array();
$params['texto'] = 'LOG das transa��es do SIGEF';
$params['tipo'] = 'log';
$params['url'] = 'spo.php?modulo=relatorio/logs/sigef&acao=A';
montaBotaoInicio($params);

$params = array();
$params['texto'] = 'LOG das transa��es do DOCUMENTA';
$params['tipo'] = 'log';
$params['url'] = 'spo.php?modulo=relatorio/logs/documenta&acao=A';
montaBotaoInicio($params);
?>
            </div>
            <div id="divEspelho" class="divCap" style="cursor: pointer;background-color: yellowgreen">
                <span class="tituloCaixa">A��es</span>
<?php
$params = array();
$params['texto'] = 'Espelho das A��es';
$params['tipo'] = 'espelho';
$params['url'] = 'spo.php?modulo=principal/espelhoacoes/espelho&acao=A';
montaBotaoInicio($params);

$params = array();
$params['texto'] = 'Extrato completo das A��es ' . $_SESSION['exercicio'];
$params['tipo'] = 'snapshot';
$params['url'] = 'spo.php?modulo=principal/dadosacoes/listadadosacoes&acao=A';
montaBotaoInicio($params);
?>
            </div>
            <br style="clear: both"/>
            <div class="divGraf"style="background-color: #EE9200">
                <span class="tituloCaixa" >Dados compartilhados entre a SPO</span> <br><br><br>
<?php
$params = array();
$params['texto'] = 'Unidades Or�ament�rias (UO)';
$params['tipo'] = 'listar';
$params['url'] = '';
montaBotaoInicio($params);

$params = array();
$params['texto'] = 'Unidades Gestoras (UG)';
$params['tipo'] = 'listar';
$params['url'] = '';
montaBotaoInicio($params);

$params = array();
$params['texto'] = 'Planos Internos (PI)';
$params['tipo'] = 'listar';
$params['url'] = '';
montaBotaoInicio($params);

$params = array();
$params['texto'] = 'Planos de trabalho Resumidos (PTRES)';
$params['tipo'] = 'listar';
$params['url'] = '';
montaBotaoInicio($params);

$params = array();
$params['texto'] = 'A��es';
$params['tipo'] = 'listar';
$params['url'] = '';
montaBotaoInicio($params);

$params = array();
$params['texto'] = 'Suba��es';
$params['tipo'] = 'listar';
$params['url'] = '';
montaBotaoInicio($params);

$params = array();
$params['texto'] = 'Contas';
$params['tipo'] = 'listar';
$params['url'] = '';
montaBotaoInicio($params);
?>
            </div>
            <div id="divComunicados" class="divCap" style="cursor: pointer;">
                <span class="tituloCaixa">Documentos (Webservices / Etc.)</span>
<?php
$params = array();
$params['texto'] = 'Editar Documentos';
$params['tipo'] = 'listar';
$params['url'] = 'spo.php?modulo=principal/documento/listar&acao=A';
montaBotaoInicio($params);

$params = array();
$params['texto'] = 'Cadastrar Documento';
$params['tipo'] = 'listar';
$params['url'] = 'spo.php?modulo=principal/documento/cadastrar&acao=A';
montaBotaoInicio($params);
?>
                <span class="subTituloCaixa">&nbsp;</span>
                <?php
                $sql = "SELECT
                        con.arqid,
                        con.angdsc
                    FROM
                        spo.anexogeral con
                    INNER JOIN
                        public.arquivo arq
                    ON
                        con.arqid = arq.arqid
                    INNER JOIN
                        seguranca.usuario usu
                    ON
                        usu.usucpf = arq.usucpf
                    WHERE
                        con.angtipoanexo = 'SU'
                    ORDER BY
                        angtipoanexo";
                $listadocumento = $db->carregar($sql);
                if ($listadocumento !== FALSE) :
                    ?>
                    <?php $listadocumento = is_array($listadocumento) ? $listadocumento : Array(); ?>
                    <?php foreach ($listadocumento as $documento) : ?>
                        <div class="btnNormal" onclick="abrirArquivo('<?php echo $documento['arqid'] ?>');" style="cursor: pointer; cursor: hand;">
                            <span class="iconeBotao glyphicon glyphicon-book" style="color: red" ></span>
        <?php echo $documento['angdsc']; ?>
                        </div>
                        <?php endforeach; ?>
                <?php else : ?>
                    <p style="clear:both;color:#FFF"><br/> N�o existem documentos cadastrados.</p>
                <?php endif; ?>
            </div>

        </td>
    </tr>
</table>

<div class="modal fade" id="qualitativo" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal">
                    <span aria-hidden="true">&times;</span><span class="sr-only">Close</span>
                </button>
                <h4 class="modal-title">Sof Qualitativo</h4>
            </div>
            <div class="modal-body">
                <div class="btn-group" style="width:100%;">
                    <button type="button" class="btn btn-primary" style='width:90%;'>Op��es</button>
                    <button type="button" class="btn btn-primary dropdown-toggle" data-toggle="dropdown" aria-expanded="false" style='width:10%;'>
                        <span class="caret"></span>
                        <span class="sr-only">Toggle Dropdown</span>
                    </button>
                    <ul class="dropdown-menu" role="menu" style="width:100%;">
<?php $menus = carregarMenusBox($_SESSION['sisid'], 3810, $_SESSION['usucpf']); ?>
                        <?php foreach ($menus as $key => &$menu) : ?>
                            <li>
                                <a href="<?php echo $menu['mnulink']; ?>" tabindex="0" role="button">
                                    <span class="glyphicon glyphicon-cloud-download"></span> <?php echo $menu['mnudsc']; ?>
                                </a>
                            </li>
<?php endforeach; ?>
                    </ul>
                </div>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-danger" data-dismiss="modal"><span class='glyphicon glyphicon-remove'></span> Fechar</button>
            </div>
        </div><!-- /.modal-content -->
    </div><!-- /.modal-dialog -->
</div><!-- /.modal -->


<div class="modal fade" id="quantitativo" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal">
                    <span aria-hidden="true">&times;</span><span class="sr-only">Close</span>
                </button>
                <h4 class="modal-title">Sof Quantitativo</h4>
            </div>
            <div class="modal-body">
                <div class="btn-group" style="width:100%;">
                    <button type="button" class="btn btn-primary" style='width:90%;'>Op��es</button>
                    <button type="button" class="btn btn-primary dropdown-toggle" data-toggle="dropdown" aria-expanded="false" style='width:10%;'>
                        <span class="caret"></span>
                        <span class="sr-only">Toggle Dropdown</span>
                    </button>
                    <ul class="dropdown-menu" role="menu" style='width:100%;'>
<?php $menus = carregarMenusBox($_SESSION['sisid'], 3820, $_SESSION['usucpf']); ?>
                        <?php foreach ($menus as $key => &$menu) : ?>
                            <li>
                                <a href="<?php echo $menu['mnulink']; ?>" tabindex="0" role="button">
                                    <span class="glyphicon glyphicon-cloud-download"></span> <?php echo $menu['mnudsc']; ?>
                                </a>
                            </li>
<?php endforeach; ?>
                    </ul>
                </div>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-danger" data-dismiss="modal"><span class='glyphicon glyphicon-remove'></span> Fechar</button>
            </div>
        </div><!-- /.modal-content -->
    </div><!-- /.modal-dialog -->
</div><!-- /.modal -->