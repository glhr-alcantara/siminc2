<?php

$cIdoc = new Public_Controller_Idoc();
$mIdoc = new Public_Model_Idoc($_REQUEST['idoid']);

switch ($_REQUEST['req']) {
    case 'salvar-idoc':
        $cIdoc->salvar($_REQUEST);
        break;
    case 'excluir-idoc':
        $mIdoc->excluir($_REQUEST['idoid']);
        break;
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
                        <input type="hidden" name="req" id="req" value="salvar-natureza" />
                        <input name="idoid" id="idoid" type="hidden" value="<?php echo $mIdoc->idoid; ?>">

                        <?php
                        echo $simec->input('idocod', 'C?digo', $mIdoc->idocod, ['class' => 'inteiro', 'required', 'maxlength' => 8]);
                        echo $simec->input('idodsc', 'Descri??o', $mIdoc->idodsc, ['required', 'maxlength' => 255]);
//                        echo $simec->select('gndcod', 'GND', $mNaturesaDespesa->gndcod, (new Public_Model_Gnd())->recuperarlistaGnd(), ['required']);
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