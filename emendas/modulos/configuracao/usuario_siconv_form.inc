<?php

$cUsuarioSiconv = new Emendas_Controller_UsuarioSiconv();
$mUsuarioSiconv = new Emendas_Model_UsuarioSiconv();
switch ($_REQUEST['req']) {
    case 'salvar':
        $cUsuarioSiconv->salvar($_REQUEST);
        die;
}


$aUsuarioSiconv = $mUsuarioSiconv->RetornaConfiguracaoSiconv();
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
                    <h5>Dados de Configuração do Usuário SICONV</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />
                        <input name="ussid" id="ussid" type="hidden" value="<?php echo $aUsuarioSiconv['ussid']; ?>">
                        
                        <?php                         
                            echo $simec->input('usucpf', 'CPF', $aUsuarioSiconv['usucpf'], ['maxlength' => 11, 'class'=>'inteiro']);                        
                            echo $simec->input('ususenhasiconv', 'Senha SICONV', base64_decode($aUsuarioSiconv['ususenhasiconv']), ['type'=>'password']);
                        ?>
                        
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="button" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=configuracao/usuario_siconv_form&acao=A" class="btn btn-warning" id="btn-voltar" type="submit"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>      
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>
<script>
    $(document).ready(function(){
        $("#btn-salvar").click(function(){
            usucpf = $("#usucpf").val().trim();
            if (!validar_cpf(usucpf)){
                swal('Atenção!', 'O número do CPF está incorreto!', 'warning');
                return;
            }
            $("#formulario").submit();
        });
    });
</script>