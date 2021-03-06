<?php

if(!empty($_FILES['arquivo'])){
    $cEmendas = new Emendas_Controller_Emenda();
    $cEmendas->importar($_FILES['arquivo']);

}


include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-12">
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
                    <form class="form-horizontal" enctype="multipart/form-data" method="post">
                        <div class="form-group">
                            <label for="inputEmail3" class="col-sm-2 control-label">Planilha SIOP</label>
                            <div class="col-sm-8">
                                <input type="file" name="arquivo" class="form-control">
                            </div>
                            <div class="col-sm-2">
                                <button type="submit" class="btn btn-success">Importar Arquivo</button>
                            </div>
                        </div>
                    </form>

                </div>
            </div>
        </div>
    </div>
</div>