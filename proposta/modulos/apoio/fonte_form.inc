<?php

$cFonte = new Public_Controller_FonteRecurso();
$mFonte = new Public_Model_FonteRecurso($_REQUEST['fonid']);

switch ($_REQUEST['req']) {
    case 'salvar-fonte':
        $cFonte->salvar($_REQUEST);
        break;
    case 'excluir-fonte':
        $cFonte->excluir($_REQUEST['fonid']);
        break;
    case 'valida-codigo':
        $resposta = $cFonte->validaFonte();
        echo $resposta;
        die;
}

include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Gerais</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar-fonte" />
                        <input name="fonid" id="fonid" type="hidden" value="<?= $mFonte->fonid; ?>">

                        <div id="codigo-fonte">
                            <?php echo $simec->input('foncod', 'Código', $mFonte->foncod, ['class' => 'inteiro' ,'required', 'maxlength' => 3]); ?>
                        </div>
                        <?php
                            echo $simec->input('fondsc', 'Descrição', $mFonte->fondsc, ['required' ,'maxlength' => 150]);
                            echo $simec->select('grfid', 'Grupo Fonte', $mFonte->grfid, (new Public_Model_GrupoFonte())->recuperarlistaGrupoFonte(), ['required']);
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/fonte&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mFonte->fonid){ ?>
                                    <a href="?modulo=apoio/fonte_form&acao=A&req=excluir-fonte&fonid=<?php echo $mFonte->fonid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>

<script>
    $(function(){
        $('#codigo-fonte').change(function () {

            $.ajax({
                url: 'proposta.php?modulo=apoio/fonte_form&acao=A&req=valida-codigo&foncod='+ $('#foncod').val()+ '&fonid='+ $('#fonid').val(),
                dataType: 'json',
                success: function(response){
                    if(response.type == 'error'){
                        swal({
                            title: response.title,
                            text: response.text,
                            type: response.type
                        });
                        $('#foncod').val('');
                    }
                }
            });
        });
    });
</script>