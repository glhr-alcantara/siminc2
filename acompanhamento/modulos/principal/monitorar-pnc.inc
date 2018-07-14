<?php

    include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";

    $listaPerfis = pegaPerfilGeral();
    
    $cAcompanhamentopnc = new Acompanhamento_Controller_Acompanhamentopnc();
    
    $mpnid = $_REQUEST['mpnid'];
    
    switch ($_REQUEST['req']) {
        case 'acompanhamento-indicador-pnc-form':
            include_once APPRAIZ. 'acompanhamento/modulos/principal/acompanhamento_indicador_pnc_form.inc';
        die;
        case 'salvar-acompanhamento-indicador-pnc':
            $resultado = $cAcompanhamentopnc->salvarIndicadorPnc($_REQUEST);
            echo json_encode($resultado);
        die;
        case 'listar-indicadores-pnc':
            include APPRAIZ. 'acompanhamento/modulos/principal/listar-indicadores-pnc.inc';
        die;
        case 'listar-atividades-pnc':
            include APPRAIZ. 'acompanhamento/modulos/principal/listar-atividades-pnc.inc';
        die;
        case 'detalhe-pi':
            include_once APPRAIZ . 'acompanhamento/modulos/principal/detalhe-pi.inc';
        die;
        case 'exibir-grafico':
            include_once APPRAIZ. 'acompanhamento/modulos/principal/modal-grafico-pnc.inc';
        die;
        case 'ver-historico':
//            $cAcompanhamentopnc->recuperarHistorico($_REQUEST['acpid']);
//            die;
        case 'uploadAnexo':
//            $file = new FilesSimec();
//            if($file->setUpload($_REQUEST['arqdescricao'], '', false)){
//                echo simec_json_encode(array(
//                    'arqid' => $file->getIdArquivo(),
//                    'arqnome' => $_FILES['file']['name'],
//                    'arqdescricao' => $_REQUEST['arqdescricao']
//                ));
//            }else{
//                echo simec_json_encode(array(
//                    'error' => 1,
//                    'errorMensage' => 'Não foi possível enviar o arquivo!'
//                ));
//            }
//        die;
    }

    $acompanhamentopnc = new Acompanhamento_Model_Acompanhamentopnc($_REQUEST['acpid']);
//ver($acompanhamentopnc, d);
    // Recuperando Janela Ativa
    $janela = (new Acompanhamento_Model_Janela())->recuperarJanelaAtiva(Acompanhamento_Model_Tipo::K_TIPO_INDICADOR_PNC);
    $janid = $acompanhamentopnc->janid = $acompanhamentopnc->janid? $acompanhamentopnc->janid: $janela['janid'];

    // Recuperando SubUnidade
    $suoid = $acompanhamentopnc->suoid? $acompanhamentopnc->suoid: $_REQUEST['suoid'];
    
//    $listaDeAnexos = (new Acompanhamento_Model_AcompanhamentoArquivo())->buscarPorAcompanhamento($acompanhamentopnc->acpid);

    if($mpnid){
        # Busca acompanhamento Ativo de acordo com a Unidade, Janela e Meta.
        $stdAcompanhamento = Acompanhamento_Controller_Acompanhamentopnc::buscarAcompanhamentoJanelaSubunidadeMeta($suoid, $janid, $mpnid);
        
        # Verifica se existe acompanhamento da meta selecionada
        if($stdAcompanhamento->acpid){
            # Carrega dados do acompanhamento da Meta
            $acompanhamentopnc = new Acompanhamento_Model_Acompanhamentopnc($stdAcompanhamento->acpid);
        } else {
            # Cria um novo registro do acompanhamento da Meta
            $acompanhamentopnc = new Acompanhamento_Model_Acompanhamentopnc();
            $acompanhamentopnc->suoid = $suoid;
            $acompanhamentopnc->janid = $janid;
            $acompanhamentopnc->mpnid = $mpnid;
            $acompanhamentopnc->salvar();
            $acompanhamentopnc->commit();
        }
    }
    
    include APPRAIZ . "includes/cabecalho.inc";
?>

<script src="js/principal/acompanhamento.js?v=1"></script>

<script src="../library/liquify/snap.svg.js"></script>
<script src="../library/liquify/liquid.meter.js"></script>

<style>
    .no-margin-bottom .form-group{
        margin-bottom: 5px;
    }
</style>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-12">
        <h2>MONITORAMENTO DAS METAS PNC</h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            
            <div class="ibox float-e-margins">
            	<div class="ibox-title">
                    <div class="row">
                        <div class="col-sm-2">
                            <h5>Metas</h5>
                        </div>
                        <div class="col-sm-10">
                            <div style="float: right;">
                                <div class="box-legenda">
                                    Legenda:
                                </div>
                                <div class="box-legenda red-bg">
                                    Pendente
                                </div>
                                <div class="box-legenda navy-bg">
                                    Preenchida
                                </div>
                                <div class="box-legenda blue-bg">
                                    Selecionada
                                </div>
                            </div>
                        </div>
            	    </div>
            	</div>
            	<div class="ibox-content">
                    <div class="row form-horizontal">

                        <div class="col-sm-6">
                            <?php
                                $listaUnidades = (new Public_Model_SubUnidadeOrcamentaria())->recuperarPorUsuario();
                                echo $simec->select('suoid', 'Unidade', $suoid, $listaUnidades, null, ['label-size'=>1]);
                            ?>
                        </div>

                        <div class="clearfix"></div>

                        <?php
                            $listaMetas = $acompanhamentopnc->getMetasPnc((object) array(
                                'suoid' => $suoid,
                                'janid' => $janid,
                                'exercicio' => $_SESSION['exercicio']
                            ));
                        
                            foreach($listaMetas as $meta){
                                $cor = $meta['acpid']? 'navy-bg': 'red-bg';
                                $cor = $mpnid == $meta['mpnid']? 'blue-bg': $cor;
                                $urlAcompanhamento = $meta['acpid']? "?modulo=principal/monitorar-pnc&acao=A&acpid={$meta['acpid']}": "?modulo=principal/monitorar-pnc&acao=A&mpnid={$meta['mpnid']}&suoid={$suoid}";
                            ?>
                                <a class="selecionar-meta-pnc" href="<?php echo $urlAcompanhamento; ?>">
                                    <div class="col-xs-1" title="<?php echo $meta['mpncod'] . ' - ' . $meta['mpndsc']; ?>">
                                        <div data-cor="<?php echo $cor ?>" class="widget div-indicador <?php echo $cor ?>">
                                            <div class="row">
                                                <div class="col-xs-12">
                                                    <h2><?php echo $meta['mpncod']; ?></h2>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                </a>
                            <?php } ?>

                        <div class="clearfix"></div>

                        <?php if($mpnid): ?>
                            <?php
                                $simec->setPodeEditar(0);
                                $valoresOrcamentarios = (new Pi_PlanoInterno())->recuperarValoresPiMetaPncESubunidade($mpnid, $suoid);
                            ?>
                            <input type="hidden" id="mpnid" value="<?php echo $mpnid; ?>" />
                            <hr />
                            
                            <div class="col-sm-12">
                                <div style="float: right;">
                                    <?php echo $simec->input('Referência', 'Referência', "{$janela['jandsc']} ({$janela['janinicio']} - {$janela['janfim']})"); ?>
                                </div>
                            </div>
                            <div class="col-sm-12">
                                <table class="table">
                                    <thead>
                                        <tr>
                                            <th>
                                                <a class="detalhe-pi" href="?modulo=principal/monitorar-pnc&acao=A&req=detalhe-pi&mpnid=<?php echo $mpnid; ?>&suoid=<?php echo $suoid; ?>" title="Exibir Lista de Planos Internos">
                                                    Planos Internos (PI´s)
                                                </a>
                                            </th>
                                            <th><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">Previsto</a></th>
                                            <th><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">Empenhado</a></th>
                                            <th><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">Liquidado</a></th>
                                            <th><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">Pago</a></th>
                                        </tr>
                                    </thead>
                                    <?php if($valoresOrcamentarios): ?>
                                        <tbody>
                                            <tr>
                                                <td>
                                                    <a class="detalhe-pi" href="?modulo=principal/monitorar-pnc&acao=A&req=detalhe-pi&mpnid=<?php echo $mpnid; ?>&suoid=<?php echo $suoid; ?>" title="Exibir Lista de Planos Internos">
                                                        <?php echo number_format($valoresOrcamentarios['qtd'], 0, ',', '.'); ?>
                                                    </a>
                                                </td>
                                                <td><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">R$ <?php echo number_format($valoresOrcamentarios['previsto'], 2, ',', '.'); ?></a></td>
                                                <td><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">R$ <?php echo number_format($valoresOrcamentarios['empenhado'], 2, ',', '.'); ?></a></td>
                                                <td><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">R$ <?php echo number_format($valoresOrcamentarios['liquidado'], 2, ',', '.'); ?></a></td>
                                                <td><a class="a_grafico" href="#" title="Exibir Grafico de Planos Internos">R$ <?php echo number_format($valoresOrcamentarios['pago'], 2, ',', '.'); ?></a></td>
                                            </tr>
                                        </tbody>
                                    <?php else: ?>
                                        <tfoot>
                                            <tr>
                                                <td>
                                                    <div class="alert alert-danger">Nenhum PI cadastrado para essa indicador</div>
                                                </td>
                                            </tr>
                                        </tfoot>
                                    <?php endif; ?>
                                </table>
                            </div>
                        <?php endif; ?>

                    </div>
            	</div>
            </div>
            
        </div>

        <?php if($mpnid): ?>
            <?php $meta = new Public_Model_MetaPnc($mpnid); ?>
            <div class="col-md-12">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <div class="row">
                            <div class="col-sm-1" style="text-align: center;">
                               <h1><?php echo $meta->mpncod; ?></h1>
                            </div>
                            <div class="col-sm-11">
                                <h2><?php echo $meta->mpndsc; ?></h2>
                            </div>
                        </div>
                    </div>
                    <div class="ibox-content">

                        <!-- STEPS INICIO -->
                        <div id="wizard">
                            <h1>INDICADORES</h1>
                            <div data-mode="async" data-url="?modulo=principal/monitorar-pnc&acao=A&req=listar-indicadores-pnc&acpid=<?php echo $acompanhamentopnc->acpid; ?>">
                                Carregando ...
                            </div>

                            <h1>ATIVIDADES</h1>
                            <div data-mode="async" data-url="?modulo=principal/monitorar-pnc&acao=A&req=listar-atividades-pnc&acpid=<?php echo $acompanhamentopnc->acpid; ?>">
                                Carregando ...
                            </div>

                            <h1>ANEXOS</h1>
                            <div>
                                Carregando ...
                            </div>
                        </div>
                        <!-- STEPS FIM -->

                    </div>
                </div>
            </div>
        <?php endif; ?>

    </div>
</div>

<?php
    bootstrapPopup(
        "Grafico de execução dos Planos Internos(PIs) da Unidade vinculados a essa Meta",
        'modal_grafico',
        '',
        array('fechar'),
        array('tamanho' => 'xl')
    );

    bootstrapPopup(
        "Planos Internos(PIs) da Unidade vinculados a essa Meta",
        'detalhe-pi',
        '',
        array('fechar'),
        array('tamanho' => 'lg')
    );
    
    bootstrapPopup(
        "Cadastro de acompanhamento do Indicador PNC",
        'div_modal_acompanhamento_indicador',
        '',
        array('salvar', 'fechar'),
        array('tamanho' => 'lg')
    );
    
    bootstrapPopup(
        "Atividade",
        'div_modal_acompanhamento_atividade',
        '',
        array('salvar', 'fechar'),
        array('tamanho' => 'lg')
    );
?>

<script>
    $(function(){
        
        var wizard = $("#wizard").steps({
            transitionEffect: "slide",
            labels: {
                cancel: "Cancelar",
                current: "Passo atual:",
                pagination: "Paginação",
                finish: "Concluir",
                next: "Próximo",
                previous: "Anterior",
                loading: "Carregando ..."
            }
        });
        
        // Evento do componente de exibir Número da meta
        $('.div-indicador').mouseenter(function(){
            var cor = $(this).data('cor');
            $(this).removeClass(cor).addClass('blue-bg');
        });

        // Evento do componente de exibir Número da meta
        $('.div-indicador').mouseleave(function(){
            var cor = $(this).data('cor');
            $(this).removeClass('blue-bg').addClass(cor);
        });

        $('.selecionar-meta-pnc').click(function(){
            if(!$('#suoid').val()){
                swal('', 'Por favor, escolha uma Unidade.', 'warning');
                return false;
            }
        });

        $('#suoid').change(function(){
            window.location.href = '?modulo=principal/monitorar-pnc&acao=A&suoid=' + $(this).val();
        });

        $('.detalhe-pi').click(function(){
            var url = $(this).attr('href');
            $('#detalhe-pi .modal-body').load(url);
            $('#detalhe-pi').modal();
            return false;
        });

        $('.a_grafico').click(function(){
            exibirGrafico();
            return false;
        });

    });
    
    /**
     * Exibe a janela modal do grafico de execução orçamentária Da Meta da Unidade.
     * 
     * @returns {undefined}
     */
    function exibirGrafico(){
        var suoid = $('#suoid').val();
        var mpnid = $('#mpnid').val();
        var url = '?modulo=principal/monitorar-pnc&acao=A&req=exibir-grafico'+ '&suoid='+ suoid+ '&mpnid='+ mpnid;
        $('#modal_grafico .modal-body').load(url);
        $('#modal_grafico').modal();
    }

    /**
     * Carrega a lista de indicadores PNC via AJAX.
     * 
     * @param {type} acpid
     * @returns {undefined}
     */
    function carregarListaIndicadorPnc(acpid){
        var url = '?modulo=principal/monitorar-pnc&acao=A&req=listar-indicadores-pnc'+ '&acpid='+ acpid;
        $('#div_lista_indicador_pnc').load(url);
    }

</script>