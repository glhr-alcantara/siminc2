<?php
    $acpid = $_REQUEST['acpid'];
    $acompanhamentopnc = new Acompanhamento_Model_Acompanhamentopnc($acpid);
    $listaIndicadorPnc = $acompanhamentopnc->buscarIndicadoresPnc();
    $qtdIndicadorPreenchido = 0;
    
    # Busca informações da Unidade e Subunidade pra exibir informações extras pra o usuário
    $subunidade = new Public_Model_SubUnidadeOrcamentaria($acompanhamentopnc->suoid);
    $unidadeOrcamentaria = new Public_Model_UnidadeOrcamentaria($subunidade->unoid);
    $exercicio = (int)$unidadeOrcamentaria->prsano;
    
    $listaPerfis = pegaPerfilGeral();
?>

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
                        
                        if($ano == $exercicio && $acompanhamentoIndicadorPnc->idpid){
                            $qtdIndicadorPreenchido++;
                        }
                ?>
                    <td>
                        <?php if(
                                in_array(PFL_SUPERUSUARIO, $listaPerfis)
                                || in_array(PFL_ADMINISTRADOR, $listaPerfis)
                                || in_array(PFL_PNC, $listaPerfis)
                                || (in_array(PFL_UNIDADE, $listaPerfis) && $ano == $exercicio)
                            ):
                        ?>
                            <?php if($acompanhamentoIndicadorPnc->idpid): ?>
                                <a href="#" class="editar_indicador link" data-idpid="<?php echo $acompanhamentoIndicadorPnc->idpid; ?>" title="Editar Indicador PNC">
                                    <?= !$acompanhamentoIndicadorPnc->idpquantidade? 0: $acompanhamentoIndicadorPnc->idpquantidade ?>
                                </a>
                            <?php else: ?>
                                <span class="editar_indicador fa fa-pencil link" data-idpid="" data-acpid="<?php echo $acpid; ?>" data-unocod="<?php echo $unidadeOrcamentaria->unocod; ?>" data-suocod="<?php echo $subunidade->suocod; ?>" data-ipncod="<?php echo trim($indicador['ipncod']); ?>" data-prsano="<?php echo $ano; ?>" title="Editar Indicador PNC"></span>
                            <?php endif; ?>
                        <?php else: ?>
                            <?php if($acompanhamentoIndicadorPnc->idpid): ?>
                                <?= !$acompanhamentoIndicadorPnc->idpquantidade? 0: $acompanhamentoIndicadorPnc->idpquantidade ?>
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

<?php
//ver($qtdIndicadorPreenchido, count($listaIndicadorPnc));
    echo '<input id="hid_indicadores_preenchidos" type="hidden" value="'.($qtdIndicadorPreenchido >= count($listaIndicadorPnc)?1:0). '" />';
?>