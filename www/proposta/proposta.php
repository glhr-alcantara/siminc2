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

// -- Export de XLS autom�tico da Listagem
Simec_Listagem::monitorarExport($_SESSION['sisdiretorio']);

# Inclus�o de classes externas
include_once APPRAIZ. 'wssof/classes/Ws_MomentosDto.inc';

//Carrega as fun��es de controle de acesso
include_once "controleAcesso.inc";
