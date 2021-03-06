<?php
/**
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */

//Declara��o de Objetos
$modelo = new Spo_Service_Periodo();

/**
 * Cabecalho padr�o do Sistema.
 * @see cabecalho.inc
 */
require(APPRAIZ . 'includes/cabecalho.inc');
$tipo = $_GET['tipo'];
switch($tipo){
    case 'acao':
        $arquivo = 'periodoAcao.inc';
        $pagina = 'Per�odo de Acompanhamento de A��es';
        break;
    case 'subacao':
        $arquivo = 'periodoSubacao.inc';
        $pagina = 'Per�odo de Acompanhamento de Suba��es';
        break;
    case 'relatorio':
        $arquivo = 'periodoRelatorioTCU.inc';
        $pagina = 'Per�odo de Acompanhamento de Relat�rio do TCU';
        break;
    case 'receita':
        $arquivo = 'periodoReceitaOrc.inc';
        $pagina = 'Per�odo de Altera��o de Receita Or�ament�ria';
        break;
    case 'alteracao':
        $arquivo = 'periodoAlteracaoCred.inc';
        $pagina = 'Momento de Altera��es de Cr�dito';
        break;
    case 'ploa':
        $arquivo = 'periodoProposta.inc';
        $pagina = 'Per�odo de Preechimento da Proposta (PLOA '.(((int)$_SESSION['exercicio'])+1).')';
        break;

}
?>
<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="">Per�odo</li>
        <li class=""><?=$pagina?></li>
    </ol>
<?php
require(dirname(__FILE__) . "/views/". $arquivo);
?>
</section>