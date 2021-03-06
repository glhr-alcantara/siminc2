<?php

$aAcao = (new Monitora_Model_Acao)->recuperarDados();

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

                    <a class="btn btn-small btn-warning" href="?modulo=apoio/acao_form&acao=A">Novo</a>

                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <thead>
                            <tr class="text-center">
                                <th>A��es</th>
                                <th>Programa</th>
                                <th>A��o</th>
                                <th>Localizador</th>
                                <th>Objetivo</th>
                                <th>Unidade Or�ament�ria</th>
                                <th>Nome</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php foreach($aAcao as $dados){ ?>
                                <tr>
                                    <td class="text-center">
                                        <a title="Alterar" href="?modulo=apoio/acao_form&acao=A&acaid=<?php echo $dados['acaid']; ?>"><i class="fa fa-pencil"></i></a>
                                    </td>
                                    <td><?php echo $dados['prgcod']; ?></td>
                                    <td><?php echo $dados['acacod']; ?></td>
                                    <td><?php echo $dados['loccod']; ?></td>
                                    <td><?php echo $dados['acaobjetivocod']? $dados['acaobjetivocod']: '-'; ?></td>
                                    <td><?php echo $dados['unidade']; ?></td>
                                    <td><?php echo $dados['acatitulo']; ?></td>
                                </tr>
                            <?php } ?>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
