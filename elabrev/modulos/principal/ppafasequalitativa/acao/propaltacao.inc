<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   M�dulo:propaltacao.inc
   Finalidade: permitir a proposta de altera��o do ppa
   */
$modulo=$_REQUEST['modulo'] ;//
if ($_REQUEST['acaid']) $_SESSION['acaid'] = $_REQUEST['acaid'];
else $_REQUEST['acaid'] =$_SESSION['acaid'];
$lista=$_REQUEST['lista'];

if ($_REQUEST['acaid'] and $_REQUEST['peajustificativa'] and $_REQUEST['acao_n']=='E')
{
	// h� proposta de exclus�o de a��o
	$sql = "select acaid from elabrev.proposta_exclusao_acao where acaid =".$_REQUEST['acaid']."  and usucpf='".$_SESSION['usucpf']."'";
    $RS = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS);
    if ($nlinhas <0)	
	   $sql = "insert into elabrev.proposta_exclusao_acao (acaid,usucpf,peajustificativa, prsano) values (".$_REQUEST['acaid'].",'".$_SESSION['usucpf']."','".$_REQUEST['peajustificativa']."','".$_SESSION['exercicio']."')";
	else
	  $sql = "update elabrev.proposta_exclusao_acao set peajustificativa = '".$_REQUEST['peajustificativa']."' where acaid=".$_REQUEST['acaid']." and usucpf='".$_SESSION['usucpf']."' ";
      $res = $db->executar($sql);
      $db->commit();    
      $db->sucesso($modulo,"&lista=E");
}
if ($_REQUEST['acaid'] and $_REQUEST['pmajustificativa'] and $_REQUEST['prgidmigra'] and $_REQUEST['acao_n']=='M')
{
	// h� proposta de migra��o de a��o
	$sql = "select acaid from elabrev.proposta_migracao_acao where acaid =".$_REQUEST['acaid']."  and usucpf='".$_SESSION['usucpf']."' ";
    $RS = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS);
    if ($nlinhas <0)	
	   $sql = "insert into elabrev.proposta_migracao_acao (acaid,usucpf,pmajustificativa,prgid,prsano) values (".$_REQUEST['acaid'].",'".$_SESSION['usucpf']."','".$_REQUEST['pmajustificativa']."',".$_REQUEST['prgidmigra'].",'".$_SESSION['exercicio']."')";
	else
	  $sql = "update elabrev.proposta_migracao_acao set pmajustificativa = '".$_REQUEST['pmajustificativa']."',prgid=".$_REQUEST['prgidmigra']." where acaid=".$_REQUEST['acaid']." and usucpf='".$_SESSION['usucpf']."' ";
      $res = $db->executar($sql);
      $db->commit();
      $db->sucesso($modulo,"&lista=M");
}

if ($_REQUEST['acao_n'] == 'excluimigra' and $_REQUEST['codid'] >0 )
{
	// h� proposta de exclus�o da proposta de migra��o
	$sql = "delete from elabrev.proposta_migracao_acao where pmaid =".$_REQUEST['codid'];
    $res = $db->executar($sql);
    $db->commit();
    $db->sucesso($modulo,"&lista=M");
}
if ($_REQUEST['acao_n'] == 'excluipea' and $_REQUEST['codid'] >0 )
{
	// h� proposta de exclus�o da proposta de exclusao
	$sql = "delete from elabrev.proposta_exclusao_acao where peaid =".$_REQUEST['codid'];
    $res = $db->executar($sql);
    $db->commit();
    $db->sucesso($modulo,"&lista=E");
}

if ($_REQUEST['decisaomigra'])
{
	// decidiu
	$momento = $db->verifica_momento();
	if ($momento==1)
	{
		$sql = "update elabrev.proposta_migracao_acao set pmastatus='A', usucpfdecisor=null where acaid=".$_SESSION['acaid']." and usucpfdecisor='".$_SESSION['usucpf']."' and prsano='".$_SESSION['exercicio']."' ";
		$db->executar($sql);
	    $sql = "update elabrev.proposta_migracao_acao set pmastatus='D',usucpfdecisor='".$_SESSION['usucpf']."' where pmaid=".$_REQUEST['decisaomigra']; 
	    $db->executar($sql);
	}
	else if ($momento==2) 
	{  
		$sql = "update elabrev.proposta_migracao_acao set pmastatusspo='A', usucpfdecisorspo=null where acaid=".$_SESSION['acaid']." and usucpfdecisorspo='".$_SESSION['usucpf']."' and prsano='".$_SESSION['exercicio']."' ";
		$db->executar($sql);
		// verifico se o usu�rio � coordenador da uma. se for, ent�o o status = Z
		if ($db-> ehcoorduma($_SESSION['usucpf']))
		$sql = "update elabrev.proposta_migracao_acao set pmastatusspo='Z',usucpfdecisorspo='".$_SESSION['usucpf']."' where pmaid=".$_REQUEST['decisaomigra'];  
		else 
		$sql = "update elabrev.proposta_migracao_acao set pmastatusspo='S',usucpfdecisorspo='".$_SESSION['usucpf']."' where pmaid=".$_REQUEST['decisaomigra']; 			 
  		$db->executar($sql);
	}
	$db->commit();
	?>
      <script>
         alert('Grava��o com sucesso.')
        // window.close();
      </script>
    <?
 //exit();
}
if ($_REQUEST['cdecisaomigra'])
{
	// cancelou a decis�o
	$momento = $db->verifica_momento();
	if ($momento==1)
	$sql = "update elabrev.proposta_migracao_acao set pmastatus='A',usucpfdecisor=null where pmaid=".$_REQUEST['cdecisaomigra']; 
	else if ($momento==2) $sql = "update elabrev.proposta_migracao_acao set pmastatusspo='A',usucpfdecisorspo=null where pmaid=".$_REQUEST['cdecisaomigra']; 	
  	$db->executar($sql);
   $db->commit();
	   ?>
      <script>
         alert('Grava��o com sucesso.')
         //window.close();
      </script>
  <?
 // exit();
}

if ($_REQUEST['decisaoexclui'])
{
	// decidiu

	$momento = $db->verifica_momento();
	if ($momento==1)
	{
		$sql = "update elabrev.proposta_exclusao_acao set peastatus='A', usucpfdecisor=null where acaid=".$_SESSION['acaid']." and usucpfdecisor='".$_SESSION['usucpf']."' and prsano='".$_SESSION['exercicio']."' ";
		$db->executar($sql);
	    $sql = "update elabrev.proposta_exclusao_acao set peastatus='D',usucpfdecisor='".$_SESSION['usucpf']."' where peaid=".$_REQUEST['decisaoexclui']; 
	    $db->executar($sql);
	}
	else if ($momento==2) 
	{  
		$sql = "update elabrev.proposta_exclusao_acao set peastatusspo='A', usucpfdecisorspo=null where acaid=".$_SESSION['acaid']." and usucpfdecisorspo='".$_SESSION['usucpf']."' and prsano='".$_SESSION['exercicio']."' ";
		$db->executar($sql);
		if ($db-> ehcoorduma($_SESSION['usucpf']))
		     $sql = "update elabrev.proposta_exclusao_acao set peastatusspo='Z',usucpfdecisorspo='".$_SESSION['usucpf']."' where peaid=".$_REQUEST['decisaoexclui'];  	
		else 
			$sql = "update elabrev.proposta_exclusao_acao set peastatusspo='S',usucpfdecisorspo='".$_SESSION['usucpf']."' where peaid=".$_REQUEST['decisaoexclui'];   
  		$db->executar($sql);
	}
	$db->commit();
	?>
      <script>
         alert('Grava��o com sucesso.')
        // window.close();
      </script>
    <?
 //exit();
}
if ($_REQUEST['cdecisaoexclui'])
{
	// cancelou a decis�o
	$momento = $db->verifica_momento();
	if ($momento==1)
	$sql = "update elabrev.proposta_exclusao_acao set peastatus='A', usucpfdecisor=null where peaid=".$_REQUEST['cdecisaoexclui']; 
	else if ($momento==2) $sql = "update elabrev.proposta_exclusao_acao set peastatusspo='A',usucpfdecisorspo=null where peaid=".$_REQUEST['cdecisaoexclui']; 	
  	$db->executar($sql);
    $db->commit();
   ?>
      <script>
         alert('Grava��o com sucesso.')
         //window.close();
      </script>
  <?
 // exit();
}

//Aqui guardo as vari�veis importantes que definem o perfil, o momento e os direitos de decis�o
$momento = $db->verifica_momento();
$criadorma = $db->ehcriador($_SESSION['acaid'],'MA'); // criador da migra��o
$criadorea = $db->ehcriador($_SESSION['acaid'],'EA'); // criador da exclus�o
$decisor = $db->ehdecisor($_SESSION['acaid'],'A',$momento); // verifica se � decisor
if ($decisor) $digit = $decisor;
else $digit = $db->ehdigitador($_SESSION['acaid'],'A',$momento); // verifica se � digitador


//Cabe�alho do sistema
include APPRAIZ."includes/cabecalho.inc";

$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Proposta de Migra��o ou Exclus�o da A��o';
monta_titulo($titulo_modulo,'');
$sql= "select a.prgcod, p.prgdsc, p.prgid, a.acaid, a.acacod, a.acadsc from ".$_SESSION['sisdiretorio'].".ppaacao_proposta a, ".$_SESSION['sisdiretorio'].".ppaprograma_proposta p where a.prgid=p.prgid and a.acastatus='A' and a.prsano = '".$_SESSION['exercicio']."'  and a.prgid=".$_SESSION['prgid']."  and a.acaid =".$_REQUEST['acaid']." limit 1";

$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$res =  @$db->carrega_registro($RS,0);
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
$acacod1 = $acacod;
$prgcod1 = $prgcod;
?>
<title>Proposta de Migra��o ou Exclus�o</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>


<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="acacod" value="<?=$acacod?>">
<input type=hidden name="decisaomigra" value="0">
<input type=hidden name="decisaoexclui" value="0">
<input type=hidden name="cdecisaomigra" value="0">
<input type=hidden name="cdecisaoexclui" value="0">
<input type=hidden name="codid" >
<input type=hidden name="acao_n" >
<center>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <tr>
    <td align='right' class="SubTituloDireita"><b>Programa:</b></td>
    <td><?=campo_texto('prgcod1','N','','',5,5,'','');?><?=campo_texto('prgdsc','N','','',80,50,'','');?></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita"><b>C�digo da a��o:</b></td>
    <td><?=campo_texto('acacod1','N','','',5,5,'','');?>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita"><b>T�tulo da a��o:</b></td>
    <td><?=campo_texto('acadsc','N','N','',120,100,'','');?></td>
  </tr>


  </table>
 
<table>
<tr valign='top'><td ><input type="radio" value="M" name="lista" onclick="mostra_div('M')"
<?if ($lista=='M') print '   checked'?>><b>Migra��o</b></td>
<td><input type="radio" value="E" name="lista" onclick="mostra_div('E')" <?if ($lista=='E') print '   checked'?>><b>Exclus�o</b>
</tr></table>
  
<div style='display:none' id='propmigra'> 

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <tr>
  <td colspan=2 align='right' class="SubTituloDireita"><b><center>Proposta de migra��o para outro Programa:</b>
  </td>
  </tr>
  <tr>
  <td align='right' class="SubTituloDireita"><b>Proposta:</b>
  </td>
  <td>
<?
    $proposmigra=0;
	$sql = "select pmajustificativa,prgid,pmaid,pmastatus,pmastatusspo from elabrev.proposta_migracao_acao where acaid =".$_REQUEST['acaid']."  and usucpf='".$_SESSION['usucpf']."' limit 1";
    $RS = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS);
    if ($nlinhas ==0) {
    	$proposmigra=1;

    $res =  @$db->carrega_registro($RS,0);
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    	$prgidmigra=$prgid;
    }
  
  $sql = "select distinct p.prgid as codigo, p.prgcod||'-'||prgdsc as descricao from elabrev.ppaprograma p, elabrev.ppaacao_proposta a where a.acaid not in (".$_SESSION['acaid'].") and a.prgid=p.prgid order by descricao";

  $db->monta_combo('prgidmigra',$sql,'S','Escolha o Programa para onde a a��o dever� migrar','','','Escolha o programa para onde a ��o dever� migrar','');
?>
  </td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita"><b>Justificativa:</b></td>
    <td><?=campo_textarea('pmajustificativa','S','S','',80,5,'','dica','');
      if ($proposmigra and $momento and $criadorma and $pmastatus=='A' and $pmastatusspo=='A')
        {
        	print '<input type="button" class="botao" name="btexcluimigra" value= "Excluir a proposta" onclick="exclui_migra('.$pmaid.')">&nbsp;&nbsp;&nbsp;';
        }
      ?>
     </td>  
  </tr> 
	 <?
	 if ($momento and $digit and $pmastatus !='D' and $pmastatusspo !='S')
	 {?>
        <tr ><td colspan="2" align="center">        
        <input type='button' class="botao" value='Gravar' onclick="gravar_janela('M')">
        </td></tr>
<?}?>  
  </table>
     <table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
    <td colspan="3" valign="top" align="center" class="title"><strong>Propostas de Migra��o nesta a��o</strong></td>
    </tr>
    <tr>
      <td valign="top" class="title"><strong>Op��es</strong></td>
      <td valign="top" class="title"><strong>Proponente</strong></td>     
      <td valign="top" class="title"><strong>Migrar para</strong></td> 
    </tr>
   </thead>
   <tbody>
   <?
	$sql="select p.pmaid,p.pmastatus,p.pmastatusspo, p.usucpf,p.usucpfdecisor,p.usucpfdecisorspo,'<img border=\"0\" src=\"../imagens/IconeAjuda.gif\" title=\" Ver justificativa \" onclick=\"ver_justificativa(' || p.pmaid|| ')\">' as verjustificativa, pr.prgcod||'-'||pr.prgdsc as programa, case when p.usucpf !='".$_SESSION['usucpf']."'  then '<img border=\"0\" src=\"../imagens/email.gif\" title=\" Enviar E-mail \" onclick=\"envia_email(' || '\'' || p.usucpf || '\'' || ')\">&nbsp;'||u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum else u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum end as autor, case when p.usucpfdecisor is not null then u2.usunome else '' end as decisoruo, case when p.usucpfdecisorspo is not null then u3.usunome else '' end as decisorspo from elabrev.proposta_migracao_acao p inner join seguranca.usuario u on p.usucpf=u.usucpf inner join elabrev.ppaprograma_proposta pr on p.prgid=pr.prgid left join seguranca.usuario u2 on u2.usucpf=p.usucpfdecisor left join seguranca.usuario u3 on u3.usucpf=p.usucpfdecisorspo where p.pmastatus in ('A','D','S') and  p.acaid=".$_SESSION['acaid']." and p.prsano='".$_SESSION['exercicio']."' order by p.pmastatus desc";

	$rs = @$db->carregar( $sql );
    if (  $rs && count($rs) > 0 )
    { 
	   $m=-1;
       foreach ( $rs as $linha )
       {
  	    $m++;
	    foreach($linha as $k=>$v) ${$k}=$v;
	    if (fmod($m,2) == 0) $marcado = '' ; else $marcado='marcado';
   		$decisao='';
		if ($momento==1)
	    {
			// ent�o est� dentro do momento
			// verifico se j� possui decis�o
			if ($pmastatus=='D')
			{
				// ent�o algu�m j� decidiu
				// verifico se o decisor � o mesmo que est� logado. se for, ent�o pode cancelar a decis�o
				if ($usucpfdecisor==$_SESSION['usucpf'] or $db->testa_superuser())
				{
					// ent�o tem pleno acesso
					$decisao="<img border='0' src='../imagens/valida1.gif' title=' Cancelar a decis�o!' onclick=\"cancelar_decisao('".$pmaid."')\">";
				}
				else 
				{			
					// neste caso, tem decis�o mas n�o � quem decidiu, logo s� pode ver
					$decisao="<img border='0' src='../imagens/valida1.gif' title=' Proposta j� decidida por $decisoruo!' >";            
				}
			}
			else
			{
				// ent�o ainda n�o tem decis�o
				//verifico se pode decidir
				if ($decisor)
				{
					$decisao="<img border='0' src='../imagens/valida2.gif' title=' Decidir por esta proposta!' onclick=\"tomar_decisao('".$pmaid."')\">";
				}
			}
		}
		if ($momento==2)
	    {
			// ent�o est� dentro do momento SPO
			// verifico se j� possui decis�o
			if ($pmastatus=='D')
			{
				// ent�o algu�m j� decidiu
				$decisao2="<img border='0' src='../imagens/valida1.gif' title=' Proposta j� decidida n�vel UO por $decisoruo!' >";            
			} else $decisao2='';
			if ($pmastatusspo=='S')
			{
				// ent�o algu�m j� decidiu na SPO
				// verifico se o decisor � o mesmo que est� logado. se for, ent�o pode cancelar a decis�o
				// verifico se foi o coordenador da uma
				if ($db->ehcoorduma($usucpfdecisorspo)) {$valid='valida6.gif' ;$title='Cancelar a decis�o n�vel MEC!';} else {$valid='valida4.gif';$title='Cancelar a decis�o n�vel SPO!';}
				if ($usucpfdecisorspo==$_SESSION['usucpf'] or $db->testa_superuser())
				{
					// ent�o tem pleno acesso
					$decisao3="<img border='0' src='../imagens/$valid' title=' $title!' onclick=\"cancelar_decisao('".$pmaid."')\">";
				}
				else 
				{			
					// neste caso, tem decis�o mas n�o � quem decidiu, logo s� pode ver
					$decisao3="<img border='0' src='../imagens/$valid' title=' Proposta j� decidida na SPO por $decisorspo!' >";            
				}
			}
			else
			{
				// ent�o ainda n�o tem decis�o
				//verifico se pode decidir
				if ($decisor)
				{
					$decisao3="<img border='0' src='../imagens/valida5.gif' title=' Decidir por esta proposta n�vel SPO!' onclick=\"tomar_decisao('".$pmaid."')\">";
				}
			}
		}		
	print '<tr class="'.$marcado.'"><td>'.$verjustificativa.'&nbsp;&nbsp;'.$decisao.$decisao2.$decisao3.'</td><td>'.$autor.'</td><td>'.$programa.'</td></tr>';
  }
}
else
{
  print '<tr class="'.$marcado.'"><td></td><td>N�o h� registros de propostas de altera��o</td></tr>';
}
?>
   </tbody>
    </table>

</div>
  
<div style='display:none' id='propexclui'> 

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  
<?
    $proposexclui=0;
	$sql = "select peajustificativa,peaid,peastatus,peastatusspo from elabrev.proposta_exclusao_acao where acaid =".$_REQUEST['acaid']."  and usucpf='".$_SESSION['usucpf']."' and peaorigem='N' limit 1";

    $RS = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS);
    if ($nlinhas ==0) {
    	$proposexclui=1;
    $res =  @$db->carrega_registro($RS,0);
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    }
?>  

  <tr>
  <td colspan=2 align='right' class="SubTituloDireita"><b><center>Proposta de Exclus�o de A��o:</b>
  </td>
  </tr>  
  <tr>
  <td align='right' class="SubTituloDireita"><b>Justificativa:</b>
  </td>
        <td><?=campo_textarea('peajustificativa','S','S','',80,5,'','dica',''); 
      
         if ($proposexclui and 	$momento and $criadorea and $peastatus=='A' and $peastatusspo=='A')
        {
        	'<input type="button" class="botao" name="btexcluimigra" value= "Excluir a proposta" onclick="exclui_pea('.$peaid.')">&nbsp;&nbsp;&nbsp;';
        }
        ?></td>  
  </tr>     
	 <?

	 if ($momento and $digit  and $peastatus !='D' and ($peastatusspo !='S' or $peastatusspo !='Z'))
	 {?>
        <tr ><td colspan="2" align="center">        
        <input type='button' class="botao" value='Gravar' onclick="gravar_janela('E')">
        </td></tr>
<?}?> 
  
 </table>
 <table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
    <td colspan="2" valign="top" align="center" class="title"><strong>Propostas de Exclus�o nesta a��o</strong></td>
    </tr>
    <tr>
      <td valign="top" class="title"><strong>Op��es</strong></td>
      <td valign="top" class="title"><strong>Proponente</strong></td>     
    </tr>
</thead>
  <tbody>
   <?
	$sql="select p.peaid,p.peastatus,p.peastatusspo,p.usucpfdecisor, p.usucpfdecisorspo, p.usucpf,'<img border=\"0\" src=\"../imagens/IconeAjuda.gif\" title=\" Ver justificativa \" onclick=\"ver_justifexclui(' || p.peaid|| ')\">' as verjustificativa, case when p.usucpf !='".$_SESSION['usucpf']."'  then '<img border=\"0\" src=\"../imagens/email.gif\" title=\" Enviar E-mail \" onclick=\"envia_email(' || '\'' || p.usucpf || '\'' || ')\">&nbsp;'||u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum else u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum end as autor, case when p.usucpfdecisor is not null then u2.usunome else '' end as decisoruo, case when p.usucpfdecisorspo is not null then u3.usunome else '' end as decisorspo from elabrev.proposta_exclusao_acao p inner join seguranca.usuario u on p.usucpf=u.usucpf left join seguranca.usuario u2 on u2.usucpf=p.usucpfdecisor left join seguranca.usuario u3 on u3.usucpf=p.usucpfdecisorspo where p.peastatus in ('A','D') and  p.acaid=".$_SESSION['acaid']." and p.prsano='".$_SESSION['exercicio']."' order by p.peastatus desc";
	$rs = @$db->carregar( $sql );
    if (  $rs && count($rs) > 0 )
    { 
	   $m=-1;
foreach ( $rs as $linha )
       {
  	    $m++;
	    foreach($linha as $k=>$v) ${$k}=$v;
	    if (fmod($m,2) == 0) $marcado = '' ; else $marcado='marcado';
   		$decisao='';
		if ($momento==1)
	    {
			// ent�o est� dentro do momento
			// verifico se j� possui decis�o
			if ($peastatus=='D')
			{
				// ent�o algu�m j� decidiu
				// verifico se o decisor � o mesmo que est� logado. se for, ent�o pode cancelar a decis�o
				if ($usucpfdecisor==$_SESSION['usucpf'] or $db->testa_superuser())
				{
					// ent�o tem pleno acesso
					$decisao="<img border='0' src='../imagens/valida1.gif' title=' Cancelar a decis�o!' onclick=\"cancelar_decisaoexc('".$peaid."')\">";
				}
				else 
				{			
					// neste caso, tem decis�o mas n�o � quem decidiu, logo s� pode ver
					$decisao="<img border='0' src='../imagens/valida1.gif' title=' Proposta j� decidida por $decisoruo!' >";            
				}
			}
			else
			{
				// ent�o ainda n�o tem decis�o
				//verifico se pode decidir
				if ($decisor)
				{
					$decisao="<img border='0' src='../imagens/valida2.gif' title=' Decidir por esta proposta!' onclick=\"tomar_decisaoexc('".$peaid."')\">";
				}
			}
		}
		if ($momento==2)
	    {
			// ent�o est� dentro do momento SPO
			// verifico se j� possui decis�o
			if ($peastatus=='D')
			{
				// ent�o algu�m j� decidiu
				$decisao2="<img border='0' src='../imagens/valida1.gif' title=' Proposta j� decidida n�vel UO por $decisoruo!' >";            
			} else $decisao2='';
			if ($peastatusspo=='S' or $peastatusspo=='Z')
			{
				// ent�o algu�m j� decidiu
				// verifico se o decisor � o mesmo que est� logado. se for, ent�o pode cancelar a decis�o
				if ($db->ehcoorduma($usucpfdecisorspo)) $valid='valida6.gif' ; else $valid='valida4.gif';
				if ($usucpfdecisorspo==$_SESSION['usucpf'] or $db->testa_superuser())
				{
					// ent�o tem pleno acesso
					$decisao3="<img border='0' src='../imagens/$valid' title=' Cancelar a decis�o n�vel SPO Decis�o de $decisorspo!' onclick=\"cancelar_decisaoexc('".$peaid."')\">";
				}
				else 
				{			
					// neste caso, tem decis�o mas n�o � quem decidiu, logo s� pode ver
					$decisao3="<img border='0' src='../imagens/$valid' title=' Proposta j� decidida na SPO por $decisorspo!' >";            
				}
			}
			else
			{
				// ent�o ainda n�o tem decis�o
				//verifico se pode decidir
				if ($decisor)
				{
					$decisao3="<img border='0' src='../imagens/valida5.gif' title=' Decidir por esta proposta n�vel SPO!' onclick=\"tomar_decisaoexc('".$peaid."')\">";
				}
			}
		}		
	print '<tr class="'.$marcado.'"><td>'.$verjustificativa.'&nbsp;&nbsp;'.$decisao.$decisao2.$decisao3.'</td><td>'.$autor.'</td></tr>';
  }
}
else
{
  print '<tr class="'.$marcado.'"><td>N�o h� registros de propostas de altera��o</td></tr>';
}
?>
   </tbody>
    </table>

</div>
<?
if ($_REQUEST['lista']=='M')
{
  ?>
<script>
   document.getElementById("propmigra").style.visibility = "visible";
   document.getElementById("propmigra").style.display = "";
</script>
<?}?>
<?
if ($_REQUEST['lista']=='E')
{
  ?>
<script>
   document.getElementById("propexclui").style.visibility = "visible";
   document.getElementById("propexclui").style.display = "";
</script>
<?}?>
</td></tr>
  </center>


    

</form>
<script>

 function gravar_janela(cod)
   {
   	   document.formulario.acao_n.value=cod;
   	   document.formulario.submit();
   }
 function exclui_pea(cod)
   {
   	if( window.confirm( "Confirma a exclus�o ?" ))
    {
   	  document.formulario.acao_n.value='excluipea';
   	  document.formulario.codid.value=cod;
   	   document.formulario.submit();
    }
   }  
    function exclui_migra(cod)
   {
   	if( window.confirm( "Confirma a exclus�o ?" ))
    {
   	   	  document.formulario.acao_n.value='excluimigra';
   	  document.formulario.codid.value=cod;
   	   document.formulario.submit();
    }
   } 
  function mostra_div(cod)
  {
    if (cod == 'M')
    {
		document.getElementById("propmigra").style.visibility = "visible";
		document.getElementById("propmigra").style.display = "";
		document.getElementById("propexclui").style.visibility = "hidden";
	    document.getElementById("propexclui").style.display = "none";
     }
    if (cod == 'E')
    {      	
		document.getElementById("propexclui").style.visibility = "visible";
		document.getElementById("propexclui").style.display = "";
		document.getElementById("propmigra").style.visibility = "hidden";
	    document.getElementById("propmigra").style.display = "none";
     }
  }
  
   function envia_email(cpf)
   {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
   }
   function ver_justificativa(cod)
   {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/ver_justifmigra&acao=A&cod="+cod;          
          window.open(e, "Justificativa","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
   }  
    
  function ver_justifexclui(cod)
   {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/ver_justifmigra&acao=A&cod="+cod+"&exc=1";          
          window.open(e, "Justificativa","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
   }
    
  function tomar_decisao(cod)
    {
    	if( window.confirm( "Confirma a decis�o?") )
         {
	         document.formulario.decisaomigra.value = cod;
	         document.formulario.submit();
         } else return;
    }   
  
  function cancelar_decisao(cod)
    {
    	if( window.confirm( "Confirma o CANCELAMENTO da decis�o?") )
         {
	         document.formulario.cdecisaomigra.value = cod;
	         document.formulario.submit();
         } else return;
    }    
    
    
  function tomar_decisaoexc(cod)
    {
    	if( window.confirm( "Confirma a decis�o?") )
         {
	         document.formulario.decisaoexclui.value = cod;
	         document.formulario.submit();
         } else return;
    }   
  
  function cancelar_decisaoexc(cod)
    {
    	if( window.confirm( "Confirma o CANCELAMENTO da decis�o?") )
         {
	         document.formulario.cdecisaoexclui.value = cod;
	         document.formulario.submit();
         } else return;
    }    
</script>

