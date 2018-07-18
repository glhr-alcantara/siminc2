<?php
define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));
require_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ."planacomorc/classes/controller/ImportarDadosSiop.inc";
$cImportaDadosSiop = new Planacomorc_Controller_ImportaDadosSiop();
$cImportaDadosSiop->AtualizarDados();
$cImportaDadosSiop->AtualizarDotacao();
