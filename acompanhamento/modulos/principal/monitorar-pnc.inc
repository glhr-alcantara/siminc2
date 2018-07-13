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
            $resultado = $cAcompanhamentopnc->listarIndicadoresPNC($_REQUEST['acpid']);
        die;
        case 'detalhe-pi':
            $cAcompanhamentopnc->recuperarPiPorMetaPncESubunidade($_REQUEST['mpnid'], $_REQUEST['suoid']);
        die;
        case 'exibir-grafico':
//            $cAcompanhamentopnc->exibirGrafico($_REQUEST['mpnid'], $_REQUEST['suoid']);
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
        # Busca acompanhamento Ativo de acordo com a Subunidade, Janela e Meta.
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
                            <h5>Indicadores</h5>
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
                                echo $simec->select('suoid', 'Unidade', $suoid, $listaUnidades, null, ['label-size'=>2]);
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

                        <?php
                            if($mpnid){
                                $simec->setPodeEditar(0);
                                
                                $valoresOrcamentarios = (new Pi_PlanoInterno())->recuperarValoresPiMetaPncESubunidade($mpnid, $suoid);
                        ?>
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
                        <?php } ?>

                    </div>
            	</div>
            </div>
            
        </div>

        <?php
            if($mpnid){
                $meta = new Public_Model_MetaPnc($mpnid);
        ?>
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
                        <form id="form" action="#" class="wizard-big wizard clearfix" role="application" novalidate="novalidate">
                            <div class="steps clearfix">
                                <ul role="tablist">
                                    <li role="tab" class="first current" aria-disabled="false" aria-selected="true">
                                        <a id="form-t-0" href="#form-h-0" aria-controls="form-p-0">
                                            <span class="current-info audible">current step: </span>
                                            <span class="number">1.</span> INDICADORES
                                        </a>
                                    </li>
                                    <li role="tab" class="disabled" aria-disabled="true">
                                        <a id="form-t-1" href="#form-h-1" aria-controls="form-p-1">
                                            <span class="number">2.</span> ATIVIDADES
                                        </a>
                                    </li>
                                    <li role="tab" class="disabled last" aria-disabled="true">
                                        <a id="form-t-2" href="#form-h-3" aria-controls="form-p-3">
                                            <span class="number">3.</span> ANEXOS
                                        </a>
                                    </li>
                                </ul>
                            </div>
                            <div class="content clearfix">
                                <h1 id="form-h-0" tabindex="-1" class="title current">Account</h1>
                                <fieldset id="form-p-0" role="tabpanel" aria-labelledby="form-h-0" class="body current" aria-hidden="false">
                                    <div class="row">
                                        <div class="col-lg-10" id="div_lista_indicador_pnc">
                                            <?php $cAcompanhamentopnc->listarIndicadoresPNC($acompanhamentopnc->acpid); ?>
                                        </div>
                                        <div class="col-lg-2">
                                            <div class="text-center">
                                                <div style="margin-top: 20px">
                                                    <i class="fa fa-sign-in" style="font-size: 180px;color: #e5e5e5 "></i>
                                                </div>
                                            </div>
                                        </div>
                                    </div>

                                </fieldset>
                                <h1 id="form-h-1" tabindex="-1" class="title">Profile</h1>
                                <fieldset id="form-p-1" role="tabpanel" aria-labelledby="form-h-1" class="body" style="display: none;" aria-hidden="true">
                                    <h2>Profile Information</h2>
                                    <div class="row">
                                        <div class="col-lg-6">
                                            <div class="form-group">
                                                <label>First name *</label>
                                                <input id="name" name="name" class="form-control required" aria-required="true" type="text">
                                            </div>
                                            <div class="form-group">
                                                <label>Last name *</label>
                                                <input id="surname" name="surname" class="form-control required" aria-required="true" type="text">
                                            </div>
                                        </div>
                                        <div class="col-lg-6">
                                            <div class="form-group">
                                                <label>Email *</label>
                                                <input id="email" name="email" class="form-control required email" aria-required="true" type="text">
                                            </div>
                                            <div class="form-group">
                                                <label>Address *</label>
                                                <input id="address" name="address" class="form-control" type="text">
                                            </div>
                                        </div>
                                    </div>
                                </fieldset>

<!--                            <div class="form-group">
                                <div class="text-center">
                                    <?php if(!in_array(PFL_CONSULTA, $listaPerfis)): ?>
                                        <input type="button" id="btn_inserir_anexos" title="Inserir Anexos" value="Inserir Anexos" class="btn btn-info"/>
                                    <?php endif; ?>
                                </div>
                            </div>-->

                                <h1 id="form-h-2" tabindex="-1" class="title">Warning</h1>
                                <fieldset id="form-p-2" role="tabpanel" aria-labelledby="form-h-2" class="body" style="display: none;" aria-hidden="true">
                                    <div class="text-center" style="margin-top: 120px">
                                        <h2>You did it Man :-)</h2>
                                    </div>
                                </fieldset>

                                <h1 id="form-h-3" tabindex="-1" class="title">Finish</h1>
                                <fieldset id="form-p-3" role="tabpanel" aria-labelledby="form-h-3" class="body" style="display: none;" aria-hidden="true">
                                    <h2>Terms and Conditions</h2>
                                    <input id="acceptTerms" name="acceptTerms" class="required" aria-required="true" type="checkbox"> <label for="acceptTerms">I agree with the Terms and Conditions.</label>
                                </fieldset>
                            </div>
                            <div class="actions clearfix">
                                <ul role="menu" aria-label="Pagination">
                                    <li class="disabled" aria-disabled="true">
                                        <a href="#previous" role="menuitem">Voltar</a>
                                    </li>
                                    <li aria-hidden="false" aria-disabled="false">
                                        <a href="#next" role="menuitem">Próximo</a>
                                    </li>
                                    <li style="display: none;" aria-hidden="true">
                                        <a href="#finish" role="menuitem">Concluir</a>
                                    </li>
                                </ul>
                            </div>
                        </form>
                    </div>
                </div>
            </div>

        <?php } ?>
    </div>
</div>


<!-- Div do Modal do Grafico -->
<div class="row">
    <div class="col-md-12">
        <div id="modal_grafico" class="modal fade">
            <div class="modal_dialog_grafico">
                <div class="modal-content">
                    <div class="modal-header">
                        <button class="close" aria-hidden="true" data-dismiss="modal" type="button">×</button>
                        <h4 class="modal-title">Grafico Planos Internos</h4>
                    </div>
                    <div class="modal-body">
                        <?php if($mpnid): ?>
                            <div class="col-sm-12">
                                <div class="col-md-8">
                                    <h3 class="text-center">PI's Associados à Meta</h3>
                                    <?php
                                        $listaPI = (new Pi_PlanoInterno())->recuperarValoresPiMetaPncESubunidade($mpnid, $suoid);
                                        $metaPnc = new Public_Model_MetaPnc($mpnid);
                                        $dadosGrafico = [];
                                        if(count($listaPI)){
                                            foreach ($listaPI as $key => $valor) {
                                                if(in_array($key, ['previsto', 'provisionado', 'empenhado', 'liquidado', 'pago'])){
                                                    $dadosGrafico[$key]['descricao'] = ucfirst($key);
                                                    $dadosGrafico[$key]['valor'] = $valor;
                                                    $dadosGrafico[$key]['categoria'] = $metaPnc->mpncod;
                                                }
                                            }
                                        }

                                        $colors = "'#55BF3B', '#eeaaee', '#00BFFF', '#aaeeee', '#7798BF', '#DDDF0D', '#7CCD7C', '#DF5353', '#008000', '#CD0000', '#FF4500', '#ff0066', '#4B0082', '#808000', '#800000', '#2F4F4F', '#006400', '#FFA500'";
                                        $grafico = new Grafico(Grafico::K_TIPO_COLUNA, false);
                                        $grafico->setFormatoTooltip(Grafico::K_TOOLTIP_DECIMAL_0)
                                            ->setColors($colors)
                                            ->setHeight('250px')
                                            ->gerarGrafico($dadosGrafico);
                                    ?>
                                </div>
                                <div class="col-sm-2">
                                    <?php
                                        $percentual = $valoresOrcamentarios['previsto']? $valoresOrcamentarios['empenhado'] * 100 / $valoresOrcamentarios['previsto'] : 0;
                                    ?>
                                    <h3 class="text-center">Percentual de Empenho</h3>
                                    <meter style="display: none;" max="1000" value="<?php echo $percentual; ?>" id="percentual-empenhado"></meter>
                                </div>
                            </div>
                        <?php endif; ?>
                    </div>
                    <div class="modal-footer">
                        <button class="btn btn-default" data-dismiss="modal" type="button">Fechar</button>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<?php
//    bootstrapPopup(
//        "Grafico Planos Internos",
//        'modal_grafico',
//        '',
//        array('fechar'),
//        array('tamanho' => 'xl')
//    );

    bootstrapPopup(
        "Planos Internos(PIs) da Subunidade vinculados a essa Meta",
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
?>

<script>
    $(function(){
        
        $('#div_lista_indicador_pnc').on('click', '.editar_indicador', function(){
            var idpid = $(this).attr('data-idpid');
            var acpid = $(this).attr('data-acpid');
            var unocod = $(this).attr('data-unocod');
            var suocod = $(this).attr('data-suocod');
            var ipncod = $(this).attr('data-ipncod');
            var prsano = $(this).attr('data-prsano');
            var url = '?modulo=principal/monitorar-pnc&acao=A'+
                '&req=acompanhamento-indicador-pnc-form'+
                '&idpid='+ idpid+
                '&acpid='+ acpid+
                '&unocod='+ unocod+
                '&suocod='+ suocod+
                '&ipncod='+ ipncod+
                '&prsano='+ prsano;

            $('#div_modal_acompanhamento_indicador').find('div.modal-body').empty();
            $('#div_modal_acompanhamento_indicador').find('div.modal-body').load(url);
            $('#div_modal_acompanhamento_indicador').modal();
            return false;
        });
        
        $('#div_modal_acompanhamento_indicador').find('.btn-salvar').click(function(){
            $('#formulario-acompanhamento-indicador').find('[type="submit"]').trigger('click');
        });

        $('.selecionar-meta-pnc').click(function(){
            if(!$('#suoid').val()){
                swal('', 'Por favor, escolha uma Subunidade.', 'warning');
                return false;
            }
        });

        $('#suoid').change(function(){
            window.location.href = '?modulo=principal/monitorar-pnc&acao=A&suoid=' + $(this).val();
        });

        $('.div-indicador').mouseenter(function(){
            var cor = $(this).data('cor');
            $(this).removeClass(cor).addClass('blue-bg');
        });

        $('.div-indicador').mouseleave(function(){
            var cor = $(this).data('cor');
            $(this).removeClass('blue-bg').addClass(cor);
        });

        $('.detalhe-pi').click(function(){
            var url = $(this).attr('href');
            $('#detalhe-pi .modal-body').load(url);
            $('#detalhe-pi').modal();
            return false;
        });

        $('#percentual-empenhado').liquidMeter({
            shape: 'circle',
            color: '#0088cc',
            background: '#F9F9F9',
            fontSize: '24px',
            fontWeight: '600',
            stroke: '#F2F2F2',
            textColor: '#333',
            liquidOpacity: 0.9,
            liquidPalette: ['#333'],
            speed: 3000
        });

        $('.a_grafico').click(function(){
            abrirModalGrafico();
            return false;
        });

    });
    
    function abrirModalGrafico() {
//        var url = $(this).attr('href');
//        $('#detalhe-pi .modal-body').load(url);
//        $('#detalhe-pi').modal();
//
        $('.modal_dialog_grafico').show();
        $('#modal_grafico').modal();
        $('#modal_grafico .chosen-container').css('width', '100%');
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