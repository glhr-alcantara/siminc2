<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html lang="pt-BR" xml:lang="pt-BR" xmlns="http://www.w3.org/1999/xhtml">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<title><?php echo SIGLA_SISTEMA; ?> - Monitoramento Estrat�gico</title>
	<link href="./cockpit/barra_do_governo/css/barra_do_governo.css" rel="stylesheet" media="screen" type="text/css" />
	<link rel="stylesheet" type="text/css" media="all" href="./cockpit/css/principal_fnde.css">
	<link href="./cockpit/images/favicon.ico" rel="shortcut icon" type="image/x-icon"/>
	<style media="all">#barra-brasil-v3-marca { width:1000px }</style>
	<script src="./cockpit/js/cufon-yui.js" type="text/javascript"></script>
	<script src="./cockpit/js/Segoe_WP_N_Light_300.font.js" type="text/javascript"></script>
	<script type="text/javascript">
		//Cufon.replace('h1'); // Works without a selector engine
		Cufon.replace('.ed_basica'); 
		Cufon.replace('.ed_profissional');
		Cufon.replace('.ed_superior');
		Cufon.replace('.txt_programa');
	</script>
</head>
<body>
	<div id="barra-brasil-v3" class="barraGoverno">		
		<div id="barra-brasil-v3-marca">Brasil &ndash; Governo Federal &ndash; Minist&eacute;rio da Educa&ccedil;&atilde;o</div>
	</div>
	<div class="wrapper">
		<div class="borda">
			<div class="container">
				<div class="page">
					<div class="section">
						<div class="row">
							<div class="column col4of5">
								<div class="content">
									<div class="marca">
										<h1 class="simec">
										<a tabindex="1" title="Voltar � p�gina inicial" class="txtIndent" href="estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A">Simec</a>
										</h1>
									</div>
								</div>
							</div>
							<div class="column col1of5 lastColumn">
								<div class="content">
									<div class="atualizar">
									<a tabindex="2" title="Atualizar" class="txtIndent" href="estrategico.php?modulo=principal/painel_cockpit_fnde&acao=A">Atualizar</a>
									</div>
								</div>
								<div>	
									<a href="wap/"><img src="cockpit/images/mobileicon2.png" /></a>
									<p >..<font color=#FFFFFF>Mobile</p>
								</div>	
								
							</div>
						</div>
						<div class="row">
							<div class="column col1of1">
								<div class="content">
									<div class="img_monitoramento">
									<a tabindex="1" title="Voltar � p�gina Monitoramento Estrat�gico" class="txtIndent" href="estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A">Monitoramento Estrat�gico</a>
									</div>
								</div>
							</div>
						</div>
						<br>
						<div class="content">
							<div class="agenda">Agendas</div>
						</div>
						<div class="row">
							<div class="column col1of3">
								<div class ="content2">
									<div class="ag_programa" onclick="window.location.href='estrategico.php?modulo=principal/cockpit_viver_sem_limites&acao=A'" style="cursor:pointer;">
										<div class="img_programa"><img src="./cockpit/images/img_viver_sem_limite.jpg" alt="Viver Sem Limite" width="316" height="67"></div>
										<div class="txt_programa">Viver Sem Limite</div>
									</div>
								</div>
							</div>
							<div class="column col1of3">
								<div class ="content2">
									<div class="ag_programa" onclick="window.location.href='estrategico.php?modulo=principal/cockpit_pronatec&acao=A'" style="cursor:pointer;">
										<div class="img_programa"><img src="./cockpit/images/img_pronatec.jpg" alt="Pronatec" width="316" height="67"></div>
										<div class="txt_programa">Pronatec</div>
									</div>
								</div>
							</div>
							<div class="column col1of3">
								<div class ="content2">
									<div class="ag_programa" onclick="window.location.href='estrategico.php?modulo=principal/cockpit_pronacampo&acao=A'" style="cursor:pointer;">
										<div class="img_programa"><img src="./cockpit/images/img_pronacampo.jpg" alt="Pronacampo" width="316" height="67"></div>
										<div class="txt_programa">Pronacampo</div>
									</div>
								</div>
							</div>
						</div>
						<div class="row">
							<div class="column col1of3">
								<div class="content">
									<div class="ed_basica">Educa��o B�sica</div>
									<div class="eb_programa" onclick="window.location.href='estrategico.php?modulo=principal/cockpit_mais_educacao&acao=A'" style="cursor:pointer;">
										<div class="img_programa"><img src="./cockpit/images/img_mais_educacao.jpg" alt="Mais Educa��o" width="316" height="67"></div>
										<div class="txt_programa">Mais Educa��o</div>
									</div>
									<div class="eb_programa" onclick="window.location.href='estrategico.php?modulo=principal/quadras_estrategico&acao=A'" style="cursor:pointer;">
										<div class="img_programa"><img src="./cockpit/images/img_quadras.jpg" alt="Constru��o e cobertura de quadras esportivas escolares" width="316" height="67"></div>
										<div class="txt_programa">Constru��o e cobertura de quadras esportivas</div>
									</div>
									<?php 
									global $db;
									if ( usuario_possui_perfil( PERFIL_SUPERUSUARIO, $_SESSION['usucpf'] ) ) { ?>
									<div class="eb_programa" onclick="window.location.href='estrategico.php?modulo=principal/cockpit_estrategico&acao=A'" style="cursor:pointer;">
										<div class="img_programa"><img src="./cockpit/images/img_pro_infancia.jpg" alt="Creches e Pr�-escolas" width="316" height="67"></div>
										<div class="txt_programa">Creches e Pr�-escolas</div>
									</div>
									<?php } ?>
									<div class="eb_programa" onclick="window.location.href='estrategico.php?modulo=principal/cockpit_proinfancia_execucao&acao=A'" style="cursor:pointer;">
										<div class="img_programa"><img src="./cockpit/images/img_obra.jpg" alt="Proinf�ncia Execu��o" width="316" height="67"></div>
										<div class="txt_programa">Proinf�ncia Execu��o</div>
									</div>
									<div class="eb_programa" onclick="window.location.href='estrategico.php?modulo=principal/cockpit_caminhos_escola&acao=A'" style="cursor:pointer;">
										<div class="img_programa"><img src="./cockpit/images/img_caminhos.jpg" alt="Caminho da Escola" width="316" height="67"></div>
										<div class="txt_programa">Caminho da Escola</div>
									</div>
									<div class="eb_programa" onclick="window.location.href='estrategico.php?modulo=principal/cockpit_educacao_digital&acao=A'" style="cursor:pointer;">
										<div class="img_programa"><img src="./cockpit/images/img_educacao_digital.jpg" alt="Educa��o Digital" width="316" height="67"></div>
										<div class="txt_programa">Educa��o Digital</div>
									</div>
									<div class="eb_programa" onclick="window.location.href='estrategico.php?modulo=principal/cockpit_ensino_medio_inovador&acao=A'" style="cursor:pointer;">
										<div class="img_programa"><img src="./cockpit/images/img_ensino_medio_inovador.jpg" alt="Ensino M�dio Inovador" width="316" height="67" /></div>
										<div class="txt_programa">Ensino M�dio Inovador</div>
									</div>
								</div>
							</div>
							<div class="column col1of3">
								<div class="content">
									<div class="ed_profissional">Educa��o Profissional</div>
									<div class="ep_programa" onclick="window.location.href='estrategico.php?modulo=principal/cockpit_brasil_pro&acao=A'" style="cursor:pointer;">
										<div class="img_programa"><img src="./cockpit/images/img_brasil_profissionalizado.jpg" alt="Brasil Profissionalizado" width="316" height="67"></div>
										<div class="txt_programa">Brasil Profissionalizado</div>
									</div>
								</div>
							</div>
							<div class="column col1of3 lastColumn">
								<div class="content">
									<div class="ed_superior">Educa��o Superior</div>
									<div class="es_programa" onclick="window.location.href='estrategico.php?modulo=principal/cockpit_educacao_superior&acao=A'" style="cursor:pointer;">
										<div class="img_programa"><img src="./cockpit/images/img_ed_superior.jpg" alt="Expans�o da Educa��o Superior" width="316" height="67"></div>
										<div class="txt_programa">Expans�o da Educa��o Superior</div>
									</div>
								</div>
							</div>
						</div>
						
					</div>
				</div>
			</div>
		</div>
	</div>
</body>
</html>