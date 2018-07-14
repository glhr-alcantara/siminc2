<?php
    $acpid = $_REQUEST['acpid'];
    $acompanhamentopnc = new Acompanhamento_Model_Acompanhamentopnc($acpid);
    $listaDeAnexos = $acompanhamentopnc->buscarAnexos();
    
    $listaPerfis = pegaPerfilGeral();
    
//    $podeEditar = in_array(PFL_CONSULTA, $listaPerfis)? 0:($mJanela->isAtual() == 't'?1 :0);

?>

<div id="div_lista_atividade_pnc">
    <div class="form-group">
        <div class="col-lg-6">
            <table id="table_anexos" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                <tr>
                    <td style="text-align: left;"><b>Nome do arquivo</b></td>
                    <td style="text-align: left;"><b>Descrição</b></td>
                    <td style="text-align: center;"><b>Ação</b></td>
                </tr>
                <?php 
                    if($listaDeAnexos):
                        foreach($listaDeAnexos as $anexo):
                ?>
                    <tr style="height: 30px;" class="tr_anexos_<?php echo $anexo['arqid']; ?>" >
                        <td style="text-align: left;"><a href="#" onclick="javascript:abrirArquivo('<?php echo $anexo['arqid']; ?>'); return false;" ><?php echo $anexo['arqnome']. '.'. $anexo['arqextensao']; ?></a></td>
                        <td style="text-align: left;"><?php echo $anexo['arqdescricao']; ?></td>
                        <td style="text-align: center;">
                            <input type="hidden" value="<?php echo $anexo['arqid']; ?>" name="listaAnexos[]">
                            <span class="glyphicon glyphicon-trash btnRemoverAnexos link" title="Excluir o arquivo <?php echo $anexo['arqnome']. '.'. $anexo['arqextensao']; ?>" data-anexos="<?php echo $anexo['arqid']; ?>" />
                        </td>
                    </tr>
                <?php
                        endforeach;
                    endif;
                ?>
            </table>
        </div>
    </div>

    <?php if(
            in_array(PFL_SUPERUSUARIO, $listaPerfis)
            || in_array(PFL_ADMINISTRADOR, $listaPerfis)
            || in_array(PFL_PNC, $listaPerfis)
            || (in_array(PFL_UNIDADE, $listaPerfis) && $ano == $exercicio)
        ):
    ?>
        <div class="form-group">
            <div class="text-center">
                <input type="button" id="btn_inserir_anexos" title="Inserir Anexos" value="Inserir Anexos" class="btn btn-info"/>
            </div>
        </div>
    <?php endif; ?>
</div>

<script>
    $(function(){

        $('#btn_inserir_anexos').click(function(){
            abrirModalUpload();
        });

    });
</script>