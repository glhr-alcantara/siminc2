<?php
/**
 * Constantes do sistema Planejamento Or�ament�rio.
 * $Id: _constantes.php 98005 2015-05-29 20:02:53Z werteralmeida $
 */
define('SISID_PLANEJAMENTO', 157);
define('MODULO', $_SESSION['sisdiretorio']);
define("SIS_NAME", "Planejamento e Acompanhamento Or�ament�rio");
define("APPRAIZ_SISOP", APPRAIZ."/planacomorc/modulos/principal/");

define("FLUXO_MONITORAMENTOACAO", 119);

define("PFL_ADMINISTRADOR", 954);
define('PFL_SUPERUSUARIO', 955);
define("PFL_PLANEJAMENTO", 956);
define("PFL_ASPAR", 957);
define("PFL_SUBUNIDADE", 994);
define("PFL_CONSULTA", 1504);
define("PFL_CONSULTA_UNIDADE", 1503);

// -- Constantes utilizadas em: monitora/modulos/principal/planotrabalhoUN/popuphistoricoplanointernoUN.inc
define("PFL_CGSO", 1044);
define("PFL_GESTAO_ORCAMENTARIA", PFL_CGSO);
define("PFL_GESTAO_ORCAMENTARIA_IFS", 1207);
define('PFL_APOIO_GESTAO', 1063);
define('PFL_GABINETE', PFL_APOIO_GESTAO);
define('PFL_GESTOR_TRANSACAO', 1007);
define('PFL_RELATORIO_TCU', 1284);

define("ESD_EMELABORACAO", 749);
define("ESD_EMVALIDACAO", 750);
define("ESD_EMAPROVACAO", 751);
define("ESD_ENVIADOSIOP", 753); // -- tah errado, n�o corrigir
define("ESD_FINALIZADO", 752);

//--constantes workflow Fluxo de monitoramento da suba��o ##select * from workflow.estadodocumento where tpdid = 265#
//Tipo de Documento WORKFLOW PI CONVENCIONAL
define("WF_TPDID_PLANEJAMENTO_PI", 265);
define("WF_TPDID_PLANEJAMENTO_PI_FNC", 266);
# Emendas
define("WF_TPDID_BENEFICIARIO", 267);
define("AED_EMENDAS_APROVAR_PI", 4325);

define("ESD_PI_CADASTRAMENTO", 1769);
define("ESD_PI_AGUARDANDO_APROVACAO", 1770);
define("ESD_PI_AGUARDANDO_CORRECAO", 1773);
define("ESD_PI_APROVADO", 1771);
define("ESD_PI_CANCELADO", 1772);

//Tipo de Documento WORKFLOW PI FNC
define("WF_TPDID_FNC_PLANEJAMENTO_PI", 266);

define("ESD_FNC_PI_CADASTRAMENTO", 1774);
define("ESD_FNC_PI_EM_ANALISE", 1775);
define("ESD_FNC_PI_DELIBERACAO_CFNC", 1776);
define("ESD_FNC_PI_SELECIONADO_CFNC", 1777);
define("ESD_FNC_PI_APROVADO", 1778);
define("ESD_FNC_PI_AGUARDANDO_CORRECAO", 1779);
define("ESD_FNC_PI_BANCO_PROJETOS", 1780);

define("PREFIX_MINISTERIO_EDUCACAO", 26);
/* Banco de dados do FINANCEIRO */
define("PARAM_DBLINK_FINANCEIRO","dbname=dbsimecfinanceiro hostaddr= user= password= port=");

// -- Unidades or�ament�rias associadas ao MEC
define("AD", 26101); // -- Administra��o Direta
define("CAPES", 26291);
define("INEP", 26290);
define("FNDE", 26298);
define("EBSERH", 26443);
define("FIES", 74902);
define("SUPERVISAOMEC", 73107);
define("UNIDADEORCAMENTARIA_SUOCOD_FNC", 42902);

// -- E-mail de recebimento de notifica��es sobre
define('EMAIL_NOTIFICACAO_SUBACAO', $_SESSION['email_sistema']);

// -- Indica uma transa��o de cria��o de PI
define('TRANSACAO_CRIACAO_PI', 'C');
// -- Indica uma transa��o de remanejamento de PI
define('TRANSACAO_REMANEAMENTO_PI', 'R');

define('WF_TPDID_PLANACOMORC_SUBACAO', '151');

define('TPDID_RELATORIO_TCU', 203);
define('ESDID_TCU_EM_PREENCHIMENTO', 1292);
define('ESDID_TCU_ANALISE_SPO', 1293);
define('ESDID_TCU_ACERTOS_UO', 1294);
define('ESDID_TCU_CONCLUIDO', 1295);

define('PERIODO_ATUAL', 5);

// ESFERA DA A��O
define( 'ESFERA_FEDERAL_BRASIL', 1 );
define( 'ESFERA_ESTADUAL_DISTRITO_FEDERAL', 2 );
define( 'ESFERA_MUNICIPAL', 3 );
define( 'ESFERA_EXTERIOR', 4 );

// Categoria de Apropria��o
define( 'CAPCOD_CONVENIO', 45);

# Plano de Trabalho Anual
define('PTAID_LINHAS_PROGRAMATICAS', 5);
