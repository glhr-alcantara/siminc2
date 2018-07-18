<?php

$cFonte = new Public_Controller_Fonte();
$mFonte = new Public_Model_FonteRecurso($_REQUEST['foncod']);

switch ($_REQUEST['req']) {
    case 'salvar-fonte':
        $cFonte->salvar($_REQUEST);
        break;
    case 'excluir-fonte':
        $cFonte->excluir($_REQUEST['foncod']);
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
                        <input type="hidden" name="req" id="req" value="salvar-fonte" />
                        <input name="foncod" id="foncod" type="hidden" value="<?= $mFonte->foncod; ?>">
                        
                        <?php
                            echo $simec->input('foncodq', 'Código', $mFonte->fondsc, ['required', 'maxlength' => 250]);
                            echo $simec->input('fondsc', 'Descrição', $mFonte->fondsc, ['required', 'maxlength' => 250]);
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/fonte&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mFonte->foncod){ ?>
                                    <a href="?modulo=apoio/fonte_form&acao=A&req=excluir-fonte&foncod=<?php echo $mFonte->foncod; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>