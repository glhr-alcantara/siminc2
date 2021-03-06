<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="js/_termoCooperacao.js"></script>
<link href="css/_termoCooperacao.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript">
$(function(){
	$('.gerarPDF').click(function(){
		//$('#requisicao').val('gerarPDF');
		//$('#formulario').submit();
        location.href=location.href+'&pdfAction=true';
	});
});
</script>
<?php

verificaCodigoTermoSessao();

if (!isset($_GET['pdfAction'])) {
	require_once APPRAIZ . 'includes/cabecalho.inc';

    echo "<br>";
    $db->cria_aba ( $abacod_tela, $url, '' );
    monta_titulo(MODULO_NAME, '');
    montaCabecalhoUG(Array(
        'termo' => true
    ));
    echo "<br>";
    montaAbaTermoCooperacao();
}

$sqlCountSolAteracao = " select count(*) from workflow.historicodocumento where aedid = " . WF_ACAO_SOL_ALTERACAO . " and docid = (select docid from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']}) ";
$rsCountSolAlt = $db->pegaUm ( $sqlCountSolAteracao );

$dadosUG = buscarDadosUndProp ();
$dadosObjeto = buscarObjetoTermo ();
$html = '';

if (!isset($_GET['pdfAction'])) {
$html.= '
	<form method="post" name="formulario" id="formulario">
		<input type="hidden" id="tcpid" name="tcpid" value="' . $_SESSION ['elabrev'] ['tcpid'] . '" />
		<input type="hidden" id="req" name="req" value="" />
		<input type="hidden" id="requisicao" name="requisicao" value="" />
		<input type="hidden" id="aba" name="aba" value="' . $_REQUEST ['aba'] . '" />
		<input type="hidden" id="acaoAba" name="acaoAba" value="' . $_REQUEST ['aba'] . '" />

		<input type="hidden" id="abaAtual" name="abaAtual" value="' . $_REQUEST ['aba'] . '" />

		<!-- DADOS DO �RG�O OU ENTIDADE PROPONENTE -->
		<!--
		<table id="gerapdf" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100#">
			<tr id="tr_titulo">
				<td class="subtitulocentro" >Termo de Execu��o Descentralizada - Gerar PDF</td>
			</tr>
		</table>
		-->
		';
}

$html.= '
				<table id="gerapdf" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100#">
					<tr id="tr_titulo">
						<td class="subtituloDireita" width="100">N� do Termo:</td>
						<td>&nbsp;' . ($rsCountSolAlt > 0 ? $_SESSION ['elabrev'] ['tcpid'] . '.' . $rsCountSolAlt : $_SESSION ['elabrev'] ['tcpid']) . '</td>
			';

if ($dadosObjeto ['ungcodconcedente'] == UG_FNDE && in_array ( $dadosObjeto ['ungcodpoliticafnde'], array (
		UG_SECADI,
		UG_SETEC,
		UG_SEB
) )) {
	$html .= '
						<td class="subtituloDireita" width="100">N� do Processo:</td>
						<td width="200">&nbsp;' . $dadosObjeto ['tcpnumprocessofnde'] . '</td>
					';
}

$html .= '
					</tr>
				</table>
				';

$html .= '
		<table id="dados_entidade_prop" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100#">
		  <tr>
		    <td height="30" colspan="6" style="background-color: #CFCFCF; text-align:center; font-weight: bold;">DADOS DO �RG�O OU ENTIDADE PROPONENTE</td>
		  </tr>
		  <tr>
		    <td width="180" style="font-weight: bold;" class="jsCounter">%d. C�d. Und. Gestora</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. C�d. da Gest�o</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. CNPJ</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Raz�o Social</td>
		  </tr>
		  <tr>
		    <td>' . $dadosUG ['ungcod'] . '</td>
		    <td colspan="2">' . $dadosUG ['gescod'] . '</td>
		    <td colspan="2">' . formatar_cnpj($dadosUG['ungcnpj']) . '</td>
		    <td colspan="2">' . $dadosUG ['ungdsc'] . '</td>
		  </tr>
		  <tr>
		    <td height="21" colspan="2" style="font-weight: bold;" class="jsCounter">%d. Endere�o</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Bairro ou Distrito</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Munic�pio</td>
		  </tr>
		  <tr>
		    <td colspan="2">' . $dadosUG ['ungendereco'] . '</td>
		    <td colspan="2">' . $dadosUG ['ungbairro'] . '</td>
		    <td colspan="2">' . $dadosUG ['muncod'] . '</td>
		  </tr>
		  <tr>
		    <td height="21" style="font-weight: bold;" class="jsCounter">%d. UF</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. CEP</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Telefone</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. E-Mail</td>
		  </tr>
		  <tr>
		    <td>' . $dadosUG ['estuf'] . '</td>
		    <td>' . $dadosUG ['ungcep'] . '</td>
		    <td colspan="2">' . $dadosUG ['ungfone'] . '</td>
		    <td colspan="2">' . $dadosUG ['ungemail'] . '</td>
		  </tr>
		</table>';
?>
<!-- REPRESENTANTE LEGAL DO �RG�O OU ENTIDADE PROPONENTE -->
<?php
$strSQL = "
    select
        u.usunome, wh.usucpf
    from workflow.historicodocumento wh
    inner join seguranca.usuario u on (u.usucpf = wh.usucpf)
    where
        wh.docid = (select docid from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']})
        and wh.aedid = 1595
";

$responsavel = $db->pegaLinha($strSQL);

$dadosProp = recuperarResponsavelProponente();
$html .= '
		<table id="dados_proponente" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100#">
		  <tr>
		    <td colspan="6" style="background-color: #CFCFCF; text-align:center; font-weight: bold;">REPRESENTANTE LEGAL DO �RG�O OU ENTIDADE PROPONENTE</td>
		  </tr>
		  <tr>
		    <td width="214" style="font-weight: bold;" class="jsCounter">%d. CPF </td>
		    <td colspan="5" style="font-weight: bold;" class="jsCounter">%d. Nome do Representante Legal</td>
		  </tr>
		  <tr>
		    <td>' . formatar_cpf($dadosProp['usucpf']). '</td>
		    <td colspan="5">' . $dadosProp['usunome'] . '</td>
		  </tr>
		  <tr>
		    <td colspan="3" style="font-weight: bold;" class="jsCounter">%d. Endere�o</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. Bairro ou Distrito</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Munic�pio</td>
		  </tr>
		  <tr>
		    <td colspan="3">' . $dadosProp['endereco'] . '</td>
		    <td>' . $dadosProp['bairro'] . '</td>
		    <td colspan="2">' . $dadosProp['municipio'] . '</td>
		  </tr>
		  <tr>
		    <td style="font-weight: bold;" class="jsCounter">%d. UF</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. CEP</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. Telefone</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. E-Mail</td>
		  </tr>
		  <tr>
		    <td>' . $dadosProp ['estado'] . '</td>
		    <td colspan="2">' . $dadosProp ['endcep'] . '</td>
		    <td>' . $dadosProp ['fone'] . '</td>
		    <td colspan="2">' . $dadosProp ['usuemail'] . '</td>
		  </tr>
		  <tr>
		    <td style="font-weight: bold;" class="jsCounter">%d. N&ordm; da C�dula da CI</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. �rg�o Expeditor</td>
		    <td colspan="3" style="font-weight: bold;" class="jsCounter">%d. Cargo</td>
		  </tr>
		  <tr>
		    <td>' . $dadosProp ['numeroidentidade'] . '</td>
		    <td colspan="2">' . $dadosProp ['entorgaoexpedidor'] . '</td>
		    <td colspan="3">' . $dadosProp ['usufuncao'] . '</td>
		  </tr>';
    if ($responsavel) {
        $html .= '<tr>
		    <td colspan="5" style="font-weight: bold;" class="jsCounter">%d. �rea T�cnica Respons�vel</td>
		    <td width="214" style="font-weight: bold;" class="jsCounter">%d. CPF </td>
		  </tr>
		  <tr>
		    <td colspan="5">' . $responsavel ['usunome'] . '</td>
		    <td>' . formatar_cpf ( $responsavel ['usucpf'] ) . '</td>
		  </tr>';
    }
$html .= '</table>';
?>
<!-- DADOS DO �RG�O OU ENTIDADE CONCEDENTE -->
<?php
$dadoConc = buscarDadosUndConc ();

$html .= '
		<table id="dados_entidade_conc" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100#">
		  <tr>
		    <td height="30" colspan="6" style="background-color: #CFCFCF; text-align:center; font-weight: bold;">DADOS DO �RG�O OU ENTIDADE CONCEDENTE</td>
		  </tr>
		  <tr>
		    <td width="180" style="font-weight: bold;" class="jsCounter">%d. C�d. Und. Gestora</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. C�d. da Gest�o</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. CNPJ</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Raz�o Social</td>
		  </tr>
		  <tr>
		    <td>' . $dadoConc ['ungcod'] . '</td>
		    <td colspan="2">' . $dadoConc ['gescod'] . '</td>
		    <td colspan="2">' . formatar_cnpj($dadoConc['ungcnpj']) . '</td>
		    <td colspan="2">' . $dadoConc ['ungdsc'] . '</td>
		  </tr>
		  <tr>
		    <td height="21" colspan="2" style="font-weight: bold;" class="jsCounter">%d. Endere�o</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Bairro ou Distrito</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Munic�pio</td>
		  </tr>
		  <tr>
		    <td colspan="2">' . $dadoConc ['ungendereco'] . '</td>
		    <td colspan="2">' . $dadoConc ['ungbairro'] . '</td>
		    <td colspan="2">' . $dadoConc ['muncod'] . '</td>
		  </tr>
		  <tr>
		    <td height="21" style="font-weight: bold;" class="jsCounter">%d. UF</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. CEP</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Telefone</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. E-Mail</td>
		  </tr>
		  <tr>
		    <td>' . $dadoConc ['estuf'] . '</td>
		    <td>' . $dadoConc ['ungcep'] . '</td>
		    <td colspan="2">' . $dadoConc ['ungfone'] . '</td>
		    <td colspan="2">' . $dadoConc ['ungemail'] . '</td>
		  </tr>
	</table>';
?>
<!-- REPRESENTANTE LEGAL DO �RG�O OU ENTIDADE CONCEDENTE -->
<?php
$strSQL = "
    select
        u.usunome, wh.usucpf
    from workflow.historicodocumento wh
    inner join seguranca.usuario u on (u.usucpf = wh.usucpf)
    where
        wh.docid = (select docid from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']})
        and wh.aedid in (1605, 1610)
    order by wh.hstid desc limit 1
";
$responsavel = $db->pegaLinha($strSQL);
$dadosRespConced = recuperarResponsavelConcedente ();

$html .= '
		<table id="dados_concedente" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100#">
		  <tr>
		    <td colspan="6" style="background-color: #CFCFCF; text-align:center; font-weight: bold;">REPRESENTANTE LEGAL DO �RG�O OU ENTIDADE CONCEDENTE</td>
		  </tr>
		  <tr>
		    <td width="214" style="font-weight: bold;" class="jsCounter">%d. CPF </td>
		    <td colspan="5" style="font-weight: bold;" class="jsCounter">%d. Nome do Representante Legal</td>
		  </tr>
		  <tr>
		    <td>' . formatar_cpf ( $dadosRespConced ['usucpf'] ) . '</td>
		    <td colspan="5">' . $dadosRespConced ['usunome'] . '</td>
		  </tr>
		  <tr>
		    <td colspan="3" style="font-weight: bold;" class="jsCounter">%d. Endere�o</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. Bairro ou Distrito</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Munic�pio</td>
		  </tr>
		  <tr>
		    <td colspan="3">' . $dadosRespConced ['endereco'] . '</td>
		    <td>' . $dadosRespConced ['bairro'] . '</td>
		    <td colspan="2">' . $dadosRespConced ['municipio'] . '</td>
		  </tr>
		  <tr>
		    <td style="font-weight: bold;" class="jsCounter">%d. UF</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. CEP</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. Telefone</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. E-Mail</td>
		  </tr>
		  <tr>
		    <td>' . $dadosRespConced ['estado'] . '</td>
		    <td colspan="2">' . $dadosRespConced ['endcep'] . '</td>
		    <td>' . $dadosRespConced ['fone'] . '</td>
		    <td colspan="2">' . $dadosRespConced ['usuemail'] . '</td>
		  </tr>
		  <tr>
		    <td style="font-weight: bold;" class="jsCounter">%d. N&ordm; da C�dula da CI</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. �rg�o Expeditor</td>
		    <td colspan="3" style="font-weight: bold;" class="jsCounter">%d. Cargo</td>
		  </tr>
		  <tr>
		    <td>' . $dadosRespConced['numeroidentidade'] . '</td>
		    <td colspan="2">' . $dadosRespConced['entorgaoexpedidor'] . '</td>
		    <td colspan="3">' . $dadosRespConced['usufuncao'] . '</td>
		  </tr>';
        if ($responsavel) {
            $html .= '<tr>
		    <td colspan="5" style="font-weight: bold;" class="jsCounter">%d. Coordena��o Respons�vel</td>
		    <td width="214" style="font-weight: bold;" class="jsCounter">%d. CPF </td>
		  </tr>
		  <tr>
		    <td colspan="5">' . $responsavel['usunome'] . '</td>
		    <td>' . formatar_cpf($responsavel['usucpf']) . '</td>
		  </tr>';
        }
$html .= '</table>';
?>
<!-- OBJETO E JUSTIFICATIVA DA DESCENTRALIZA��O DO CR�DITO -->
<?php

$html .= '
		<table id="objeto_justificativa" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100#">
		  <tr>
		    <td colspan="2" style="background-color: #CFCFCF; text-align:center; font-weight: bold;">OBJETO E JUSTIFICATIVA DA DESCENTRALIZA��O DO CR�DITO</td>
		  </tr>
		  <tr>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Identifica��o (T�tulo/Objeto da Despesa)</td>
		  </tr>
		  <tr>
		    <td colspan="2">' . $dadosObjeto ['tcpdscobjetoidentificacao'] . '</td>
		  </tr>
		  <tr>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Objetivo</td>
		  </tr>
		  <tr>
		    <td colspan="2">' . $dadosObjeto ['tcpobjetivoobjeto'] . '</td>
		  </tr>
		  <tr>
		    <td width="350" style="font-weight: bold;" class="jsCounter">%d. UG/Gest�o Repassadora</td>
		    <td width="350" style="font-weight: bold;" class="jsCounter">%d. UG/Gest�o Recebedora</td>
		  </tr>
		  <tr>
		    <td style="font-weight: bold;">';

if (in_array ( $dadoConc ['ungcod'], array (
		UG_FNDE,
		UG_INEP,
		UG_CAPES
) )) {
	$html .= $dadoConc ['ungcod'] . ' / ' . $dadoConc ['ungcod'];
} else {
	$html .= '152734 / 00001';
}

$html .= '</td>
		    <td>' . $dadosUG ['ungcod'] . ' / ' . $dadosUG ['gescod'] . '</td>
		  </tr>
		  <tr>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Justificativa (Motiva��o/Clientela/Cronograma Fisico)</td>
		  </tr>
		  <tr>
		    <td colspan="2">' . $dadosObjeto ['tcpobjetojustificativa'] . '</td>
		  </tr>
		  <tr>
		    <td colspan="2">';

// $dadosObjeto['ungcodproponente'] == UG_FNDE || (
if ($dadosObjeto ['ungcodconcedente'] == UG_FNDE && in_array ( $dadosObjeto ['ungcodpoliticafnde'], array (
		UG_SECADI,
		UG_SETEC,
		UG_SEB
) )) {
	$html .= '
		    	<p>I - Integra este termo, independentemente de transcri��o, o Plano de Trabalho e o Termo de Refer�ncia, cujos dados ali contidos acatam os part�cipes e se comprometem em cumprir, sujeitando-se �s normas da Lei Complementar n� 101/2000, Lei n� 8.666, de 21 de junho de 1993, no que couber, Lei n� 4.320/1964, Lei n� 10.520/2002, Decreto n� 93.872/1986 e o de n� 6.170, de 25 de julho de 2007, Portaria Interministerial no 507, de 24 de novembro de 2011, Portaria Conjunta MP/MF/CGU n� 8, de 7 de novembro de 2012, bem como o disposto na Resolu��o CD/FNDE n� 28/2013.<br/></p>
				<p>
		    	II - constituem obriga��es da CONCEDENTE:<br/>
				a) efetuar a transfer�ncia dos recursos financeiros previstos para a execu��o deste Termo, na forma estabelecida no Cronograma de Desembolso constante do Plano de Trabalho; <br/>
		    	</p>
		    	<p>
				III - constituem obriga��es do GESTOR DO PROGRAMA:<br/>
				a) orientar, supervisionar e cooperar com a implanta��o das a��es objeto deste Termo;<br/>
				b) acompanhar as atividades de execu��o, avaliando os seus resultados e reflexos;<br/>
				c) analisar o relat�rio de cumprimento do objeto do presente Termo;<br/>
	    		</p>
		    	<p>
				IV - constituem obriga��es da PROPONENTE:<br/>
				a) solicitar ao gestor do projeto senha e login do SIMEC;<br/>
				b) solicitar � UG concedente senha e login do SIGEFWEB, no caso de recursos enviados pelo FNDE;<br/>
				c) promover a execu��o do objeto do Termo na forma e prazos estabelecidos no Plano de Trabalho;<br/>
				d) aplicar os recursos discriminados exclusivamente na consecu��o do objeto deste Termo;<br/>
				e) permitir e facilitar ao �rg�o Concedente o acesso a toda documenta��o, depend�ncias e locais do projeto;<br/>
				f) observar e exigir, na apresenta��o dos servi�os, se couber, o cumprimento das normas espec�ficas que regem a forma de execu��o da a��o a que os cr�ditos estiverem vinculados;<br/>
				g) manter o �rg�o Concedente informado sobre quaisquer eventos que dificultem ou interrompam o curso normal de execu��o do Termo;<br/>
				h) devolver os saldos dos cr�ditos or�ament�rios descentralizados e n�o empenhados, bem como os recursos financeiros n�o utilizados, conforme norma de encerramento do correspondente exerc�cio financeiro;<br/>
				i) emitir o relat�rio descritivo de cumprimento do objeto proposto;<br/>
				j) comprovar o bom e regular emprego dos recursos recebidos, bem como dos resultados alcan�ados;<br/>
				k) assumir todas as obriga��es legais decorrentes de contrata��es necess�rias � execu��o do objeto do termo;<br/>
				l) solicitar ao gestor do projeto , quando for o caso, a prorroga��o do prazo para cumprimento do objeto em at� quinze (15) dias antes do t�rmino previsto no termo de execu��o descentralizada, ficando tal prorroga��o condicionada � aprova��o por aquele;<br/>
				m) a presta��o de contas dos cr�ditos descentralizados devem integrar as contas anuais do �rg�o Proponente a serem apresentadas aos �rg�os de controle interno e externo, conforme normas vigentes;<br/>
				n) apresentar relat�rio de cumprimento do objeto pactuado at� 60 dias ap�s o t�rmino do prazo para cumprimento do objeto estabelecido no Termo.<br/>
		    	</p>';
} else {
	$html .= '
		    	<p>I - Integra este termo, independentemente de transcri��o, o Plano de Trabalho e o Termo de Refer�ncia, cujos dados ali contidos acatam os part�cipes e se comprometem em cumprir, sujeitando-se �s normas da Lei Complementar n� 101/2000, Lei n� 8.666, de 21 de junho de 1993, no que couber, Lei n� 4.320/1964, Lei n� 10.520/2002, Decreto n� 93.872/1986 e o de n� 6.170, de 25 de julho de 2007, Portaria Interministerial no 507, de 24 de novembro de 2011, Portaria Conjunta MP/MF/CGU n� 8, de 7 de novembro de 2012, bem como o disposto na Resolu��o CD/FNDE n� 28/2013.<br/></p>
		    	<p>
				II - constituem obriga��es da CONCEDENTE:<br/>
				a) efetuar a transfer�ncia dos recursos financeiros previstos para a execu��o deste Termo, na forma estabelecida no Cronograma de Desembolso constante do Plano de Trabalho;<br/>
				b) orientar, supervisionar e cooperar com a implanta��o das a��es objeto deste Termo;<br/>
				c) acompanhar as atividades de execu��o, avaliando os seus resultados e reflexos;<br/>
				d) analisar o relat�rio de cumprimento do objeto do presente Termo;<br/>
		    	</p>
		    	<p>
				III - constituem obriga��es da PROPONENTE:<br/>
				a) solicitar ao gestor do projeto senha e login do SIMEC;<br/>
				b) solicitar � UG concedente senha e login do SIGEFWEB, no caso de recursos enviados pelo FNDE;<br/>
				c) promover a execu��o do objeto do Termo na forma e prazos estabelecidos no Plano de Trabalho;<br/>
				d) aplicar os recursos discriminados exclusivamente na consecu��o do objeto deste Termo;<br/>
				e) permitir e facilitar ao �rg�o Concedente o acesso a toda documenta��o, depend�ncias e locais do projeto;<br/>
				f) observar e exigir, na apresenta��o dos servi�os, se couber, o cumprimento das normas espec�ficas que regem a forma de execu��o da a��o a que os cr�ditos estiverem vinculados;<br/>
				g) manter o �rg�o Concedente informado sobre quaisquer eventos que dificultem ou interrompam o curso normal de execu��o do Termo;<br/>
				h) devolver os saldos dos cr�ditos or�ament�rios descentralizados e n�o empenhados, bem como os recursos financeiros n�o utilizados, conforme norma de encerramento do correspondente exerc�cio financeiro;<br/>
				i) emitir o relat�rio descritivo de cumprimento do objeto proposto;<br/>
				j) comprovar o bom e regular emprego dos recursos recebidos, bem como dos resultados alcan�ados;<br/>
				k) assumir todas as obriga��es legais decorrentes de contrata��es necess�rias � execu��o do objeto do termo;<br/>
				l) solicitar ao gestor do projeto , quando for o caso, a prorroga��o do prazo para cumprimento do objeto em at� quinze (15) dias antes do t�rmino previsto no termo de execu��o descentralizada, ficando tal prorroga��o condicionada � aprova��o por aquele;<br/>
				m) a presta��o de contas dos cr�ditos descentralizados devem integrar as contas anuais do �rg�o Proponente a serem apresentadas aos �rg�os de controle interno e externo, conforme normas vigentes;<br/>
				n) apresentar relat�rio de cumprimento do objeto pactuado at� 60 dias ap�s o t�rmino do prazo para cumprimento do objeto estabelecido no Termo.<br/>
		    	</p>';
}
$html .= '</td>
		  </tr>
		</table>';
?>
<!-- PREVIS�O OR�AMENT�RIA -->
<?php
$dadosPrevisao = buscarPrevisaoOrca ();
$html .= '
		<table id="previsao_orc" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100#">
		  <tr>
		    <td height="21" colspan="8"style="background-color: #CFCFCF; text-align:center; font-weight: bold;">PREVIS�O OR�AMENT�RIA</td>
		  </tr>
		  <tr>
		  	<td style="font-weight: bold;" class="jsCounter">%d. Ano</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. Programa de Trabalho</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. A��o</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. Plano Interno</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. Descri��o da A��o constante da LOA</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. Nat. da Despesa</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. M�s da Libera��o</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. Valor (em R$ 1,00)</td>
		  </tr>';

if ($dadosPrevisao) :
    $tot = 0;
	$arAnosPrevisao = array ();
	$totalPrevisao = count ( $dadosPrevisao ) - 1;

    $indice = 0;
	foreach ( $dadosPrevisao as $k => $d ) :

		if (!in_array ( $d ['proanoreferencia'], $arAnosPrevisao)) {
			if ($subTotalPorAno > 0) {

				$html .= '
                    <tr bgcolor="#f0f0f0">
                        <td colspan="8" align="right">
                            <table>
                                <tr>
                                    <td><b>Subtotal (' . ($anoAnterior ? $anoAnterior : 'ano n�o informado') . ')</b>&nbsp;</td>
                                    <td align="right" width="110"><b>R$ ' . number_format($subTotalPorAno, 2, ',', '.') . '</b></td>
                                    <td align="right" width="110"><b>R$ ' . number_format($subTotalPorAno, 2, '.', ',') . '</b></td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                ';
                $indice++;
			}
			array_push ( $arAnosPrevisao, $d ['proanoreferencia'] );
			$subTotalPorAno = 0;
			$anoAnterior = $d ['proanoreferencia'];
		}


        if ((int)$d['valor'] > 0) {
            $valor = $d['valor_f'];
        } else {
            if ((int)$dado['valor'] <= 0) {
                if ($db->pegaLinha("select * from elabrev.creditoremanejado where proid = {$d['proid']}"))
                    continue;
            }
        }

		$html .= '
          <tr>
            <td style="font-size:10px;">' . $d ['proanoreferencia'] . '</td>
            <td style="font-size:10px;">' . $d ['plano_trabalho'] . '</font></td>
            <td style="font-size:10px;">' . $d ['acao'] . '</td>
            <td style="font-size:10px;">' . $d ['plano_interno'] . '</td>
            <td style="font-size:10px;">' . $d ['acao_loa'] . '</td>
            <td style="font-size:10px;">' . $d ['nat_despesa'] . '</td>
            <td style="font-size:10px;">' . mes_extenso ( $d ['crdmesliberacao'] ) . '</td>
            <td style="font-size:10px;" align="right">R$ ' . $valor . '</td>
          </tr>
        ';


		$subTotalPorAno = $subTotalPorAno + trim($d['valor']);

        $tot += trim($d['valor']);
	endforeach
	;

    if ($totalPrevisao == $k) {
        $html .= '
            <tr bgcolor="#f0f0f0">
                <td colspan="8" align="right">
                    <table>
                        <tr>
                            <td><b>Subtotal (' . ($anoAnterior ? $anoAnterior : 'ano n�o informado') . ')</b>&nbsp;</td>
                            <td align="right" width="110"><b>R$ ' . number_format($subTotalPorAno, 2, '.', ',') . '</b></td>
                        </tr>
                    </table>
                </td>
            </tr>
        ';
    }
endif;

$sql = "select * from monitora.previsaoorcamentaria
		where tcpid = {$_SESSION['elabrev']['tcpid']} AND prostatus = 'A' AND crdmesexecucao IS NOT NULL
		order by proid limit 1";

$rsPOmes = $db->pegaLinha ( $sql );

$html .= '
		  <tr>
		  	<td colspan="2"><b class="jsCounter">%d. Prazo para o cumprimento do objeto</b></td>
		  	<td colspan="2">' . ($rsPOmes ['crdmesexecucao'] ? $rsPOmes ['crdmesexecucao'] . '&nbsp;meses' : '') . '&nbsp;</td>
		    <td colspan="2" align="right" style="font-weight: bold;" class="jsCounter">%d. TOTAL</td>
		    <td colspan="2" align="right">R$ ' . $d ['total'] . '</td>
		  </tr>
		</table>';
?>
<!--  ASSINATURAS -->
<?php
$mes = date ( "m" );

switch ($mes) {
	case 1 :
		$mes = "Janeiro";
		break;
	case 2 :
		$mes = "Fevereiro";
		break;
	case 3 :
		$mes = "Mar�o";
		break;
	case 4 :
		$mes = "Abril";
		break;
	case 5 :
		$mes = "Maio";
		break;
	case 6 :
		$mes = "Junho";
		break;
	case 7 :
		$mes = "Julho";
		break;
	case 8 :
		$mes = "Agosto";
		break;
	case 9 :
		$mes = "Setembro";
		break;
	case 10 :
		$mes = "Outubro";
		break;
	case 11 :
		$mes = "Novembro";
		break;
	case 12 :
		$mes = "Dezembro";
		break;
}

$sqlReitor = "select
				u.usunome,
				u.usucpf,
				to_char(h.htddata, 'DD/MM/YYYY') as htddata,
				to_char(h.htddata, 'HH:II:SS') as hora,
				g.ungdsc
			from monitora.termocooperacao t
			inner join workflow.historicodocumento h on h.docid = t.docid
			inner join workflow.acaoestadodoc a on a.aedid = h.aedid
			inner join seguranca.usuario u on u.usucpf = h.usucpf
			left join unidadegestora g on g.ungcod = t.ungcodconcedente
			where t.tcpid = {$_SESSION['elabrev']['tcpid']}
			and a.esdiddestino = " . EM_ANALISE_DA_SECRETARIA . "
			order by hstid asc";

$rsReitor = $db->pegaLinha ( $sqlReitor );
$stAnaliseReitor = '';

//mostra assinatura somente quando termo � aprovado pelo representante legal do proponente
$mostraAssinaturaReitor = (!in_array($estadoAtual, array(EM_CADASTRAMENTO, TERMO_AGUARDANDO_APROVACAO_GESTOR_PROP, EM_APROVACAO_DA_REITORIA)));

if ($rsReitor && $mostraAssinaturaReitor) {
	$stAnaliseReitor = "Autorizado pelo(a) {$rsReitor['usunome']} no dia {$rsReitor['htddata']} �s {$rsReitor['hora']} <br/>";
}

$sqlPresidente = "select * from monitora.termocooperacao where ungcodconcedente = '153173' and tcpid = {$_SESSION['elabrev']['tcpid']}";
//ver($sqlPresidente, d);
$rsPresidente = $db->pegaLinha($sqlPresidente);

$where = '';
if ($rsPresidente) {
	$where .= " and a.esdiddestino in ( " . EM_EMISSAO_NOTA_CREDITO . ", ". EM_ANALISE_PELA_SPO ." )";
} else {
	$where .= " and a.esdiddestino = " . EM_ANALISE_PELA_CGSO . " ";
}

$sqlSecretario = "select
				u.usunome,
				u.usucpf,
				to_char(h.htddata, 'DD/MM/YYYY') as htddata,
				to_char(h.htddata, 'HH:II:SS') as hora,
				g.ungdsc
			from monitora.termocooperacao t
			inner join workflow.historicodocumento h on h.docid = t.docid
			inner join workflow.acaoestadodoc a on a.aedid = h.aedid
			inner join seguranca.usuario u on u.usucpf = h.usucpf
			left join unidadegestora g on g.ungcod = t.ungcodconcedente
			where t.tcpid = {$_SESSION['elabrev']['tcpid']}
			{$where}
			order by hstid asc";
//ver($sqlSecretario, d);
$rsSecretario = $db->pegaLinha ( $sqlSecretario );
$stAnaliseSecretaria = '';

$arrayPull = array(
    EM_CADASTRAMENTO,
    TERMO_AGUARDANDO_APROVACAO_GESTOR_PROP,
    EM_APROVACAO_DA_REITORIA,
    EM_ANALISE_OU_PENDENTE,
    AGUARDANDO_APROVACAO_DIRETORIA,
    AGUARDANDO_APROVACAO_SECRETARIO,
    TERMO_EM_ANALISE_ORCAMENTARIA_FNDE,
    AGUARDANDO_APROVACAO_DIRETORIA,
);

/**
 * mostra assinatura somente quando termo �
 * autorizado pelo representante legal do concedente
 * ou
 * encaminhado para valida��o da diretoria do FNDE
 */
$mostraAssConcedente = (!in_array($estadoAtual, $arrayPull));

if ($rsSecretario && $mostraAssConcedente) {
	if ($rsPresidente) {
		$stAnaliseSecretaria = "Autorizado pelo(a) presidente(a) {$rsSecretario['usunome']} no dia {$rsSecretario['htddata']} �s {$rsSecretario['hora']}";
	} else {
		$stAnaliseSecretaria = "Autorizado pelo(a) secret�rio(a) {$rsSecretario['usunome']} no dia {$rsSecretario['htddata']} �s {$rsSecretario['hora']}";
	}
}
$html .= '
		<table id="assinatura" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100#">
		  <tr>
		    <td>&nbsp;</td>
		  </tr>
		  <tr>
		    <td align="center">
		    	' . $stAnaliseReitor . '
		    	' . $stAnaliseSecretaria . '
		    </td>
		  </tr>
		  <tr>
		    <td>&nbsp;</td>
		  </tr>
		  <tr>
		    <!-- <td colspan="2" style="text-align:center; font-weight: bold;">Bras�lia, ' . date ( "d" ) . ' de ' . $mes . ' de ' . date ( "Y" ) . '</td> -->
    		<td colspan="2" style="text-align:center; font-weight: bold;">' . recuperaDataGeraPdf () . '</td>
		  </tr>
		  <tr>
		    <td>&nbsp;</td>
		  </tr>
		</table>';

$arr = array();
$count = substr_count($html, '%d');
for ($i=1; $i<=$count; $i++) {
    $pos = strpos($html, '%d');

    if ($pos !== false) {
        $html = substr_replace($html, $i, $pos, 2);
    }
}

$html = str_replace('100#', '100%', $html);

if ($_GET['pdfAction'] == true) {

    /*define('_MPDF_URI', APPRAIZ . 'includes/mpdf60/');
    define('_MPDF_PATH', APPRAIZ . 'includes/mpdf60/');
    define('_MPDF_TEMP_PATH', sys_get_temp_dir().'/');
    define('_MPDF_TTFONTDATAPATH', sys_get_temp_dir().'/');

    include_once APPRAIZ . 'includes/mpdf60/mpdf.php';
    $html = utf8_encode($html);

    $mpdf = new mPDF();
    $mpdf->SetDisplayMode('fullpage');
    $mpdf->text_input_as_HTML = true;
    $mpdf->charset_in = 'utf-8';
    $mpdf->WriteHTML($html);
    $name = "Termo_De_Execucao_Descentralizada_n_{$_SESSION['elabrev']['tcpid']}.pdf";
    $mpdf->Output($name, 'D');*/

    include_once APPRAIZ . 'elabrev/classes/modelo/HtmlToPdf.class.inc';
    $pdfObj = new HtmlToPdf($html);
    $pdfObj->setTitle("Termo_De_Execucao_Descentralizada_n_{$_SESSION['elabrev']['tcpid']}.pdf");
    $pdfObj->getPDF();
    exit;

} else {
    echo $html;
}
?>
<table align="center" bgcolor="#f5f5f5" border="0" class="tabela"
	cellpadding="3" cellspacing="1">
	<tr>
		<td colspan="3" align="center"><input type="button" value="Gerar PDF" class="gerarPDF" /></td>
	</tr>
</table>
</form>

