<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

// carrega as fun��es espec�ficas do m�dulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';
require_once APPRAIZ . 'includes/funcoesspo.php';

$simec = new Simec_View_Helper();
$_SESSION['sislayoutbootstrap'] = 'zimec';

//Carrega as fun��es de controle de acesso
include_once "controleAcesso.inc";
?>
