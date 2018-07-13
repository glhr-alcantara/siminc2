<?php
    $acompanhamentopnc = new Acompanhamento_Model_Acompanhamentopnc($acpid);
    $listaIndicadorPnc = $acompanhamentopnc->getIndicadoresPnc();
    
    # Busca informações da Unidade e Subunidade pra exibir informações extras pra o usuário
    $subunidade = new Public_Model_SubUnidadeOrcamentaria($acompanhamentopnc->suoid);
    $unidadeOrcamentaria = new Public_Model_UnidadeOrcamentaria($subunidade->unoid);
    $exercicio = (int)$unidadeOrcamentaria->prsano;
    
    $listaPerfis = pegaPerfilGeral();
?>

<div id="div_lista_indicador_pnc">
    <table class="table table-bordered">
        <thead>
            <tr>
                <th>DESCRIÇÃO DO INDICADOR</th>
                <?php for($ano = 2010; $ano <= ((int)$exercicio); $ano++): ?>
                    <th><?php echo $ano; ?></th>
                <?php endfor; ?>
            </tr>
        </thead>
        <?php if($listaIndicadorPnc): ?>
        <tbody>
            <?php foreach($listaIndicadorPnc as $indicador): ?>
                <tr>
                    <td><?php echo $indicador['ipncod']. ' - '. $indicador['ipndsc']; ?></td>
                    <?php
                        for($ano = 2010; $ano <= ((int)$exercicio); $ano++):
                            $acompanhamentoIndicadorPnc = new Acompanhamento_Model_Indicadorpnc();
                            $acompanhamentoIndicadorPnc->acpid = $acompanhamentopnc->acpid;
                            $acompanhamentoIndicadorPnc->unocod = $unidadeOrcamentaria->unocod;
                            $acompanhamentoIndicadorPnc->suocod = $subunidade->suocod;
                            $acompanhamentoIndicadorPnc->ipncod = $indicador['ipncod'];
                            $acompanhamentoIndicadorPnc->prsano = $ano;
                            $acompanhamentoIndicadorPnc->buscarPorAcompanhamentoPncUnidadeIndicadorAno();
                    ?>
                        <td>
                            <?php if(
                                    in_array(PFL_SUPERUSUARIO, $listaPerfis)
                                    || in_array(PFL_ADMINISTRADOR, $listaPerfis)
                                    || in_array(PFL_PNC, $listaPerfis)
                                    || (in_array(PFL_UNIDADE, $listaPerfis) && $ano == $exercicio)
                                ):
                            ?>
                                <?php if($acompanhamentoIndicadorPnc->idpquantidade): ?>
                                    <a href="#" class="editar_indicador link" data-idpid="<?php echo $acompanhamentoIndicadorPnc->idpid; ?>" title="Editar Indicador PNC">
                                        <?= $acompanhamentoIndicadorPnc->idpquantidade; ?>
                                    </a>
                                <?php else: ?>
                                    <span class="editar_indicador fa fa-pencil link" data-idpid="" data-acpid="<?php echo $acpid; ?>" data-unocod="<?php echo $unidadeOrcamentaria->unocod; ?>" data-suocod="<?php echo $subunidade->suocod; ?>" data-ipncod="<?php echo trim($indicador['ipncod']); ?>" data-prsano="<?php echo $ano; ?>" title="Editar Indicador PNC"></span>
                                <?php endif; ?>
                            <?php else: ?>
                                <?php if($acompanhamentoIndicadorPnc->idpquantidade): ?>
                                    <?= $acompanhamentoIndicadorPnc->idpquantidade; ?>
                                <?php else: ?>
                                    <span class="fa fa-pencil" title="Indicador PNC"></span>
                                <?php endif; ?>
                            <?php endif; ?>
                        </td>
                    <?php endfor; ?>
                </tr>
            <?php endforeach; ?>
        </tbody>
        <?php else: ?>
            <tfoot>
                <tr>
                    <td colspan="<?php echo($ano-2010); ?>">Não existem indicadores cadastrados pra essa Meta PNC.</td>
                </tr>
            </tfoot>
        <?php endif; ?>
    </table>
</div>

<script>
    $(function(){
        // Eventos do ícone(ou número) de edição da lista de indicadores PNC
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
        
        // Evento do botão salvar da janela modal do Cadastro de Indicadores PNC
        $('#div_modal_acompanhamento_indicador').find('.btn-salvar').click(function(){
            $('#formulario-acompanhamento-indicador').find('[type="submit"]').trigger('click');
        });
    });
</script>