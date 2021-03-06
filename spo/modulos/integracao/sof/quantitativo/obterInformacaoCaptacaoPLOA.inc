<?php
/**
 * Arquivo de consulta das informa��es de capta��o da PLOA.
 *
 * $Id: obterInformacaoCaptacaoPLOA.inc 101245 2015-08-13 19:35:06Z maykelbraz $
 * @filesource
 */
$fm = new Simec_Helper_FlashMessage('spo/integracao/infocaptacao');
if (!empty($_POST) && isset($_POST['requisicao'])) {
    $requisicao = $_POST['requisicao'];
    $dados = $_POST['infocaptacao'];

    switch ($requisicao) {
        case 'carregar':
            obterInformacaoCaptacaoPLOA($dados, $fm);
            break;
    }
//    header('Location: ' . $_SERVER['REQUEST_URI']);
//    die();
}

/**
 * Cabe�alho padr�o do sistema.
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<script type="text/javascript">
function onInfocaptacaoSubmit()
{
    if ('' === $('#infocaptacao_codigoMomento').val()) {
        alert('O c�digo do momento n�o pode ser deixado em branco.');
        return false;
    }
    return true;
}
</script>
<div class="row col-md-12">
<?php
    $bc = new Simec_View_Breadcrumb();
    $bc->add('Integra��o')
        ->add('SOF Quantitativo')
        ->add('Obter Informa��es de Capta��o da PLOA')
        ->render();

    echo $fm;

    $form = new Simec_View_Form('infocaptacao');
    $form->carregarDados($_POST['infocaptacao'])
        ->addCombo('Momento', 'codigomomento', Spo_Model_Momento::queryCombo())
        ->addHidden('exercicio', $_SESSION['exercicio'])
        ->addHidden('codigoOrgao', Spo_Model_Unidade::CODIGO_ORGAO)
        ->addTexto('C�d. Tipo Detalhamento', 'codigoTipoDetalhamento', 2)
        ->addCombo('Unidade Or�ament�ria', 'codigounidadeorcamentaria', Spo_Model_Unidade::queryCombo())
        ->addChoices('Captados', 'captados', array('Todos' => null, 'Sim' => 't', 'N�o' => 'f'))
        ->addChoices('Capt�veis', 'captaveis', array('Todos' => null, 'Sim' => 't', 'N�o' => 'f'))
        ->addCheckbox('Gravar log', 'loggar')
        ->addCheckbox('Limpar tabela', 'limpar')
        ->addBotoes(array('executar'))
        ->setRequisicao('carregar')
        ->render();
    ?>
</div>
