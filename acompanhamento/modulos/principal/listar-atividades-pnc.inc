<?php
    $acpid = $_REQUEST['acpid'];
    $acompanhamentopnc = new Acompanhamento_Model_Acompanhamentopnc($acpid);
    $listaAtividades = $acompanhamentopnc->buscarAtividades();
    
    # Verifica se existe pelo menos uma atividade inserida no acompanhamento da Meta PNC
    $atividadePreenchida = $acompanhamentopnc->verificarPreenchimentoAtividade();

?>
<table class="table table-bordered">
    <thead>
        <tr>
            <th>Atividade realizadas em <?php echo (int)$_SESSION['exercicio']; ?></th>
            <th>Quantidade</th>
            <th>Produto ou serviço resultante</th>
            <th>Localização que a atividade foi realizada</th>
            <th>Fonte de verificação da informação</th>
        </tr>
    </thead>
    <?php if($listaAtividades): ?>
    <tbody>
        <?php foreach($listaAtividades as $atividade): ?>
            <tr>
                <td><?php echo $atividade['atvtitulo']; ?></td>
                <td><?= number_format($atividade['atvprodutoqtd'], 2, ',', '.'); ?></td>
                <td><?php echo $atividade['atvproduto']; ?></td>
                <td><?php echo $atividade['mundescricao']. '-'. $atividade['estuf']; ?></td>
                <td>
                    <?php if(Acompanhamento_Controller_Acompanhamentopnc::verificarPermissaoEditar($acpid)): ?>
                        <?= $atividade['atvfonteinformacao']; ?>
                        &nbsp;
                        <span class="editar_atividade fa fa-pencil link" data-atvid="<?php echo $atividade['atvid']; ?>" data-acpid="<?php echo $acpid; ?>" title="Editar Atividade"></span>
                        <span style="color: red;">
                            <span class="excluir_atividade glyphicon glyphicon-trash link" data-acpid="<?php echo $acpid; ?>" data-atvid="<?php echo $atividade['atvid']; ?>" data-atvtitulo="<?php echo $atividade['atvtitulo']; ?>" title="Excluir Atividade: <?php echo $atividade['atvtitulo']; ?>" /></span>
                        </span>
                    <?php else: ?>
                        <?= $atividade['atvfonteinformacao']; ?>
                    <?php endif; ?>
                </td>
            </tr>
        <?php endforeach; ?>
    </tbody>
    <tfoot>
        <?php if(Acompanhamento_Controller_Acompanhamentopnc::verificarPermissaoEditar($acpid)): ?>
            <tr>
                <td colspan="4"></td>
                <td style="text-align: right;">
                    <button class="btn btn-success editar_atividade" data-atvid="" data-acpid="<?php echo $acpid; ?>" type="button">
                        <i class="fa fa-plus-circle"></i>&nbsp;Inserir nova atividade
                    </button>
                </td>
            </tr>
        <?php endif; ?>
    </tfoot>
    <?php else: ?>
        <tfoot>
            <tr>
                <td colspan="5">Não existem atividades cadastradas pra essa Meta PNC.</td>
            </tr>
            <?php if(Acompanhamento_Controller_Acompanhamentopnc::verificarPermissaoEditar($acpid)): ?>
                <tr>
                    <td colspan="4"></td>
                    <td style="text-align: right;">
                        <button class="btn btn-success editar_atividade" data-atvid="" data-acpid="<?php echo $acpid; ?>" type="button">
                            <i class="fa fa-plus-circle"></i>&nbsp;Inserir nova atividade
                        </button>
                    </td>
                </tr>
            <?php endif; ?>
        </tfoot>
    <?php endif; ?>
</table>

<?php echo '<input id="hid_atividade_preenchida" type="hidden" value="'.((int)$atividadePreenchida). '" />'; ?>
