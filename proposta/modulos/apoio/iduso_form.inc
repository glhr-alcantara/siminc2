<?php

$cIduso = new Public_Controller_Iduso();
$mIduso = new Public_Model_IdentifUso($_REQUEST['iduid']);

switch ($_REQUEST['req']) {
    case 'salvar-iduso':
        $cIduso->salvar($_REQUEST);
        break;
    case 'excluir-iduso':
        $cIduso->excluir($_REQUEST['iduid']);
        break;
    case 'validar-iduso':
        $resposta = $cIduso->validarIduso();
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
                        <input type="hidden" name="req" id="req" value="salvar-iduso" />
                        <input type="hidden" name="iduid" id="iduid" value="<?php echo $mIduso->iduid?>" />

                        <div id="codigo-iduso">
                            <?php
                                echo $simec->input('iducod', 'Código', $mIduso->iducod, ['class' => 'inteiro', 'required', 'maxlength' => 1]);
                            ?>
                        </div>
                        <?php
                            echo $simec->input('idudsc', 'Descrição', $mIduso->idudsc, ['required', 'maxlength' => 100]);
                        ?>
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/iduso&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mIduso->iduid){ ?>
                                    <a href="?modulo=apoio/iduso_form&acao=A&req=excluir-iduso&iduid=<?php echo $mIduso->iduid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
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
        $('#codigo-iduso').change(function () {

            $.ajax({
                url: 'proposta.php?modulo=apoio/iduso_form&acao=A&req=validar-iduso&iducod='+ $('#iducod').val()+ '&iduid='+ $('#iduid').val(),
                dataType: 'json',
                success: function(response){
                    if(response.type == 'error'){
                        swal({
                            title: response.title,
                            text: response.text,
                            type: response.type
                        });
                        $('#iducod').val('');
                    }
                }
            });
        });
    });
</script>