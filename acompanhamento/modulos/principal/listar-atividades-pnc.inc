<?php
    $acpid = $_REQUEST['acpid'];
    $acompanhamentopnc = new Acompanhamento_Model_Acompanhamentopnc($acpid);
    $listaAtividades = $acompanhamentopnc->buscarAtividades();
    
    # Busca informações da Unidade e Subunidade pra exibir informações extras pra o usuário
    $subunidade = new Public_Model_SubUnidadeOrcamentaria($acompanhamentopnc->suoid);
    $unidadeOrcamentaria = new Public_Model_UnidadeOrcamentaria($subunidade->unoid);
    $exercicio = (int)$unidadeOrcamentaria->prsano;
    
    $listaPerfis = pegaPerfilGeral();
?>

<div id="div_lista_atividade_pnc">
    <table class="table table-bordered">
        <thead>
            <tr>
                <th>Atividade realizadas em <?php echo (int)$_SESSION['exercicio']; ?></th>
                <th>Produto ou serviço resultante</th>
                <th>Quantidade</th>
                <th>Localização que a atividade foi realizada</th>
                <th>Fonte de verificação da informação</th>
            </tr>
        </thead>
        <?php if($listaAtividades): ?>
        <tbody>
            <?php foreach($listaAtividades as $atividade): ?>
                <tr>
                    <td><?php echo $atividade['atvtitulo']; ?></td>
                    <td><?php echo $atividade['atvproduto']; ?></td>
                    <td><?= number_format($atividade['atvprodutoqtd'], 2, ',', '.'); ?></td>
                    <td><?php echo $atividade['mundescricao']. '-'. $atividade['estuf']; ?></td>
                    <td>
                        <?php if(
                                in_array(PFL_SUPERUSUARIO, $listaPerfis)
                                || in_array(PFL_ADMINISTRADOR, $listaPerfis)
                                || in_array(PFL_PNC, $listaPerfis)
                                || (in_array(PFL_UNIDADE, $listaPerfis) && $ano == $exercicio)
                            ):
                        ?>
                            <?= $atividade['atvfonteinformacao']; ?>
                            &nbsp;
                            <span class="editar_atividade fa fa-pencil link" data-atvid="<?php echo $atividade['atvid']; ?>" data-acpid="<?php echo $acpid; ?>" title="Editar Atividade"></span>
                        <?php else: ?>
                            <?= $atividade['atvfonteinformacao']; ?>
                        <?php endif; ?>
                    </td>
                </tr>
            <?php endforeach; ?>
        </tbody>
        <tfoot>
            <tr>
                <td colspan="4"></td>
                <td>
                    <button class="btn btn-success editar_atividade" data-atvid="" data-acpid="<?php echo $acpid; ?>" type="button">
                        <i class="fa fa-check"></i>&nbsp;Incluir nova atividade
                    </button>
                </td>
            </tr>
        </tfoot>
        <?php else: ?>
            <tfoot>
                <tr>
                    <td colspan="5">Não existem atividades cadastradas pra essa Meta PNC.</td>
                </tr>
                <tr>
                    <td colspan="4"></td>
                    <td style="text-align: right;">
                        <button class="btn btn-success editar_atividade" data-atvid="" data-acpid="<?php echo $acpid; ?>" type="button">
                            <i class="fa fa-check"></i>&nbsp;Incluir nova atividade
                        </button>
                    </td>
                </tr>
            </tfoot>
        <?php endif; ?>
    </table>
</div>

<script>
    $(function(){

        // Eventos do ícone(ou número) de edição da lista de atividades PNC
        $('#div_lista_atividade_pnc').on('click', '.editar_atividade', function(){
            var atvid = $(this).attr('data-atvid');
            var acpid = $(this).attr('data-acpid');
            var url = '?modulo=principal/monitorar-pnc&acao=A'+
                '&req=acompanhamento-atividade-pnc-form'+
                '&atvid='+ atvid+
                '&acpid='+ acpid;

            $('#div_modal_acompanhamento_atividade').find('div.modal-body').empty();
            $('#div_modal_acompanhamento_atividade').find('div.modal-body').load(url);
            $('#div_modal_acompanhamento_atividade').modal();
            return false;
        });
        
        // Evento do botão salvar da janela modal do Cadastro de Indicadores PNC
        $('#div_modal_acompanhamento_atividade').find('.btn-salvar').click(function(){
            $('#formulario-acompanhamento-atividade').find('[type="submit"]').trigger('click');
        });

    });
</script>