<?php
ini_set("memory_limit", "3000M");
set_time_limit(30000);

include_once "config.inc";
include_once "_constantes.php";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// abre conex�o com o servidor de banco de dados
$db = new cls_banco();

include APPRAIZ . 'includes/classes/EmailAgendado.class.inc';

$sql = "
SELECT DISTINCT
	entnome,
	entnumcpfcnpj,
	usuemail,
	to_char(dmi.dmidataexecucao, 'DD/MM/YYYY') as dmidataexecucao,
	coalesce(to_char(dmi.dmidatavalidacao, 'DD/MM/YYYY'),'N/A') as dmidatavalidacao,
	atidescricao || ' - ' || mnmdsc as descricao,
	ati._atiprojeto as projeto,
	esd.esddsc as descricao_estado,
	(SELECT acadsc from painel.acao aca2 inner join pde.atividade ati2 ON ati2.atiacaid = aca2.acaid where ati2.atiid = ati._atiprojeto) as programa,
	doc.docid  as documento,
	case 
		when doc.esdid = 443 and tpvid = 1 and dmi.dmidataexecucao <= now()::date then 'Pend�ncias'
        when doc.esdid = 444 and tpvid = 2 and dmi.dmidataexecucao <= now()::date then 'Pend�ncias'
        when doc.esdid = 445 then 'Resolvidas'
        when dmi.dmidataexecucao > now()::date then 'Futuras'
    end as pendencia,
    case 
    	when doc.esdid = 443 and tpvid = 1 and dmi.dmidataexecucao <= now()::date then 1
    	when doc.esdid = 444 and tpvid = 2 and dmi.dmidataexecucao <= now()::date then 1
    	when doc.esdid = 445 then 2
    	when dmi.dmidataexecucao > now()::date then 3
    end as ordem
FROM
                painel.detalhemetaindicador dmi
INNER JOIN painel.detalheperiodicidade 	dpe  ON dpe.dpeid = dmi.dpeid and dpestatus = 'A'
INNER JOIN pde.monitorameta 			mnm  ON mnm.metid = dmi.metid
INNER JOIN (SELECT max(mmeid) as mmeid, mnmid FROM pde.monitorametaentidade GROUP BY mnmid) mmex  ON mmex.mnmid = mnm.mnmid
INNER JOIN pde.monitorametaentidade 	mme  ON mmex.mmeid = mme.mmeid
INNER JOIN workflow.documento 			doc  ON doc.docid = dmi.docid
INNER JOIN pde.monitoraitemchecklist 	mic  ON mic.micid = mnm.micid
INNER JOIN pde.atividade 				ati  ON ati.atiid = mic.atiid               
INNER JOIN workflow.estadodocumento 	esd  ON esd.esdid = doc.esdid
INNER JOIN entidade.entidade 			resp ON mme.entid = resp.entid
INNER JOIN seguranca.usuario 			usu  ON usu.usucpf = resp.entnumcpfcnpj
WHERE
	dmi.dmistatus= 'A'
	AND mic.micstatus = 'A'
	AND ati.atistatus = 'A'
	AND mnm.mnmstatus = 'A'
	AND doc.docid is not null
	AND doc.esdid != 445
	AND 
		(
			dmi.dmidataexecucao < 
			(
			SELECT
				CASE 
					WHEN to_char(now(),'DY') = 'MON' THEN (now()::date+4)
					WHEN to_char(now(),'DY') = 'TUE' THEN (now()::date+3)
					WHEN to_char(now(),'DY') = 'WED' THEN (now()::date+2)
					WHEN to_char(now(),'DY') = 'THU' THEN (now()::date+1)
					WHEN to_char(now(),'DY') = 'FRI' THEN (now()::date)
					WHEN to_char(now(),'DY') = 'SAT' THEN (now()::date+6)
					WHEN to_char(now(),'DY') = 'SUN' THEN (now()::date+5)
					ELSE now()::date
				END
			)
		)
ORDER BY
	entnome,ordem,dmidataexecucao asc ";

$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

foreach($arrDados as $d){
	$arrUsu[$d['entnumcpfcnpj']]['email'] = $d['usuemail'];
	$arrUsu[$d['entnumcpfcnpj']]['nome'] = $d['entnome'];
	$arrUsu[$d['entnumcpfcnpj']]['projeto'] = $d['projeto'];
	$arrUsu[$d['entnumcpfcnpj']]['pendencia'][] = array(
													"data_execucao" => $d['dmidataexecucao'],
													"data_validacao" => $d['dmidatavalidacao'],
													"descricao" => $d['descricao'],
													"estado" => $d['descricao_estado'],
													"programa" => $d['programa'],
													"pendencia" => $d['pendencia'],
												);
}
$arrUsu = $arrUsu ? $arrUsu : array();

foreach($arrUsu as $u){
	$e = new EmailAgendado();
	$e->setTitle("Pend�ncias - Monitoramento Estrat�gico - SIMEC");
	$html = '
	Sr(a). '.$u['nome'].',<br />
	Voc� possui os seguintes itens na sua Caixa de Entrada do M�dulo Monitoramento Estrat�gico:<br /><br />';
	$html.= '<table bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">';
	$html.= '<tr  bgcolor="#c5c5c5" >';
	$html.= '<td align="center" ><b>N�</b></td><td align="center" ><b>Tipo</b></td><td align="center" ><b>Data da Meta</b></td><td align="center" ><b>Data da Valida��o</b></td><td align="center" ><b>Descri��o</b></td><td align="center" ><b>Programa</b></td><td align="center" ><b>Situa��o Atual</b></td>';
	$html.= '</tr>';
	$n=1;
	foreach($u['pendencia'] as $p){
		$cor = $n%2==1 ? "#ffffff" : "";
		$html.= '<tr bgcolor="'.$cor.'" >';
		$html.= '<td>'.$n.'</td><td>'.$p['pendencia'].'</td><td>'.$p['data_execucao'].'</td><td>'.$p['data_validacao'].'</td><td>'.$p['descricao'].'</td><td>'.$p['programa'].'</td><td>'.$p['estado'].'</td>';
		$html.= '</tr>';
		$n++;
	}
	$html.= '</table><br />';
	$html.= 'Atenciosamente,<br /><br />
	Equipe SIMEC<br /><br />
	Obs.: Este � um email autom�tico enviado pelo sistema, favor n�o responder.</div>';
	echo $html."<br/><br/>";
	$e->setText($html);
	$e->setName("SIMEC - Minist�rio da Educa��o");
	$e->setEmailOrigem("no-reply@mec.gov.br");
	$arrEmail = Array($u['email'],'HenriqueCouto@mec.gov.br');
	if( $u['projeto'] != '' ){
		$sql = "SELECT 
					usuemail
				FROM 
					pde.usuarioresponsabilidade ur
				INNER JOIN seguranca.usuario usu ON usu.usucpf = ur.usucpf
				WHERE
					ur.pflcod in (591,592) AND rpustatus = 'A' AND atiid = ".$u['projeto'];
		$emails = $db->carregarColuna($sql);
		foreach($emails as $email){
			$arrEmail[] = $email;
		}
	}
//	$arrEmail[] = $u['email'];
//	$arrEmail = "HenriqueCouto@mec.gov.br";
	$e->setEmailsDestino($arrEmail);
	$e->enviarEmails();	
}