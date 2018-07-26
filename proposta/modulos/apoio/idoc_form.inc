<?php

$cIdoc = new Public_Controller_Idoc();
$mIdoc = new Public_Model_Idoc($_REQUEST['idoid']);

switch ($_REQUEST['req']) {
    case 'salvar-idoc':
        $cIdoc->salvar($_REQUEST);
        break;
    case 'excluir-idoc':
        $cIdoc->excluir($_REQUEST['idoid']);
        break;
    case 'validar-idoc':
        $resposta = $cIdoc->validarIdoc();
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
                        <input type="hidden" name="req" id="req" value="salvar-idoc" />
                        <input name="idoid" id="idoid" type="hidden" value="<?php echo $mIdoc->idoid; ?>">

                        <div id="codigo-idoc">
                        <?php
                            echo $simec->input('idocod', 'Codigo', $mIdoc->idocod, ['class' => 'inteiro', 'required', 'maxlength' => 4]);
                        ?>
                        </div>
                        <?php
                            echo $simec->input('idodsc', 'Descrição', $mIdoc->idodsc, ['required', 'maxlength' => 200]);
                        ?>
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/idoc&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mIdoc->idoid){ ?>
                                    <a href="?modulo=apoio/idoc_form&acao=A&req=excluir-idoc&idoid=<?php echo $mIdoc->idoid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
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
    $(function () {
        $('#codigo-idoc').change(function () {

            $.ajax({
                url: 'proposta.php?modulo=apoio/idoc_form&acao=A&req=validar-idoc&idoid=' + $('#idoid').val() + '&idocod=' + $('#idocod').val(),
                dataType: 'json',
                success: function (response) {
                    if (response.type == 'error') {
                        swal({
                            title: response.title,
                            text: response.text,
                            type: response.type
                        });
                        $('#idocod').val('');
                    }
                }
            });
        });

    });

</script>
