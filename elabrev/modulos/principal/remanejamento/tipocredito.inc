<?php

require_once( 'tipocredito_funcoes.inc' );

switch ( $_REQUEST['acao'] )
{
	// inserir ou editar
	case 'E':
		require 'tipocredito_editar.inc';
		break;
	// remover
	// ap�s a remo��o a listagem � acionada
	case 'R':
		require 'tipocredito_remover.inc';
	// listar
	case 'L':
		require 'tipocredito_listar.inc';
		break;
	
}
