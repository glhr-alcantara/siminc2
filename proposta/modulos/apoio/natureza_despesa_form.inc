<?php

$cNaturesaDespesa = new Public_Controller_NaturezaDespesa();
$mNaturesaDespesa = new Public_Model_NaturezaDespesa($_REQUEST['ndpid']);

switch ($_REQUEST['req']) {
    case 'salvar-natureza':
        $cNaturesaDespesa->salvar($_REQUEST);
        break;
    case 'excluir-natureza':
        $cNaturesaDespesa->excluir($_REQUEST);
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
                        <input name="ndpid" id="ndpid" type="hidden" value="<?php echo $mNaturesaDespesa->ndpid; ?>">
                        
                        <?php
                            echo $simec->input('ndpcod', 'Código', $mNaturesaDespesa->ndpcod, ['class' => 'inteiro', 'required', 'maxlength' => 8]);
                            echo $simec->input('ndpdsc', 'Descrição', $mNaturesaDespesa->ndpdsc, ['required', 'maxlength' => 255]);
                            echo $simec->select('gndcod', 'GND', $mNaturesaDespesa->gndcod, (new Public_Model_Gnd())->recuperarlistaGnd(), ['required']);
                        ?>
                        
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/natureza_despesa&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mNaturesaDespesa->ndpid){ ?>
                                    <a href="?modulo=apoio/natureza_despesa_form&acao=A&req=excluir-natureza&ndpid=<?= $mAreaCultural->ndpid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>