
<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   M�dulo:propostarevisao.inc
   Finalidade: permitir a proposta de revisao 
   */
$modulo=$_REQUEST['modulo'] ;//
if ($_REQUEST['prgid']) $_SESSION['prgid']= $_REQUEST['prgid']; else $_REQUEST['prgid'] = $_SESSION['prgid'];
if ($_REQUEST['acaid']) $_SESSION['acaid']= $_REQUEST['acaid']; else $_REQUEST['acaid'] = $_SESSION['acaid'];


$titulo_modulo='Proposta de Revis�o de Atributos de A��o';
monta_titulo($titulo_modulo,'');

$sql= "select a.*, m.unmdsc, p.prodsc from elabrev.ppaacao_proposta a left join unidademedida m on a.unmcod=m.unmcod left join produto p on a.procod=p.procod where a.acastatus in ('A','N') and a.acaid = ".$_REQUEST['acaid'];

if (! $_REQUEST['acaid'] or $_REQUEST['acaid']=='') {
	?>
	<script>alert('Ocorreu um problema ao abrir a p�gina. Tente novamente!');
	history.back();</script>
	<?
	$db->close();
	exit();
}

$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);

if ($nlinhas < 0 ){
	?>
	<script>alert('A��o Inexistente!');
	history.back();</script>
	<?
	$db->close();
	exit();}
else {
  $res =  $db->carrega_registro($RS,0);

}
// Transforma em vari�veis todos os campos do array
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

if ($_REQUEST['decisao'])
{
	// decidiu
	$momento = $db->verifica_momento();
	if ($momento==1)
	{
		$sql = "update elabrev.elaboracaorevisao set eradata=current_date,erastatus='A', usucpfdecisor=null where eracod=".$_SESSION['acaid']." and eracampo='".$_REQUEST['campo']."' and usucpfdecisor='".$_SESSION['usucpf']."' and prsano='".$_SESSION['exercicio']."' ";
		$db->executar($sql);
	    $sql = "update elabrev.elaboracaorevisao set eradata=current_date,erastatus='D',usucpfdecisor='".$_SESSION['usucpf']."' where eraid=".$_REQUEST['decisao']; 
	    $db->executar($sql);
	}
	else if ($momento==2) 
	{  
		$sql = "update elabrev.elaboracaorevisao set eradata=current_date,erastatusspo='A', usucpfdecisorspo=null where eracod=".$_SESSION['acaid']." and eracampo='".$_REQUEST['campo']."' and usucpfdecisorspo='".$_SESSION['usucpf']."' and prsano='".$_SESSION['exercicio']."' ";
		$db->executar($sql);
	    $sql = "update elabrev.elaboracaorevisao set eradata=current_date,erastatusspo='S',usucpfdecisorspo='".$_SESSION['usucpf']."' where eraid=".$_REQUEST['decisao']; 
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


if ($_REQUEST['cdecisao'])
{
	// cancelou a decis�o
	$momento = $db->verifica_momento();
	if ($momento==1)
	$sql = "update elabrev.elaboracaorevisao set eradata=current_date, erastatus='A', usucpfdecisor=null where eraid=".$_REQUEST['cdecisao']; 
	else if ($momento==2) $sql = "update elabrev.elaboracaorevisao set eradata=current_date, erastatusspo='A' ,usucpfdecisorspo=null where eraid=".$_REQUEST['cdecisao'];	
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

if ($_REQUEST['grava'] and $_REQUEST['erajustificativa'])    
{
  if ($_REQUEST['origem']<>$_REQUEST['alvo'])
  {
  	// ent�o as tabelas de origem e alvo s�o diferentes. Trat-se de uma foreing key
  	$sqldsc = "select ".$_REQUEST['padrao']."dsc from ".$_REQUEST['alvo']." where ".$_REQUEST['padrao']."cod=".$_REQUEST['erapropostacod'];
    $titulo=$db->recuperar($sqldsc);
    $_REQUEST['eraproposta'] = $titulo[$_REQUEST['padrao'].'dsc'];
  }
  if ($_REQUEST['inserir']==1)
  {
  	if (! $_REQUEST['erapropostacod']) $_REQUEST['erapropostacod']=0;
  	$sql = "insert into elabrev.elaboracaorevisao (
  	  usucpf,
      eratabela,
      eracampo,
      prsano,
      eraproposta,
      erajustificativa,
      eracod,      
      erapropostacod ) values ('".
  	$_SESSION['usucpf']."','ppaacao_proposta','".
  	$_REQUEST['campo']."','".
  	$_SESSION['exercicio']."','".
  	$_REQUEST['eraproposta']."','".
  	$_REQUEST['erajustificativa']."',".
  	$_SESSION['acaid'].",".
  	$_REQUEST['erapropostacod'].")";

  	 $db->executar($sql);
	 $db->commit();
	   ?>
      <script>
         alert('Grava��o com sucesso.')
         //window.close();
      </script>
  <?
  //exit();
  }
    if ($_REQUEST['inserir']==2)
  {
  	$sql = "update elabrev.elaboracaorevisao set eradata=now() ,eraproposta='".$_REQUEST['eraproposta']."',erapropostacod='".$_REQUEST['erapropostacod']."',erajustificativa='".$_REQUEST['erajustificativa']."' where usucpf='".$_SESSION['usucpf']."' and eratabela='ppaacao_proposta' and eracampo='".$_REQUEST['campo']."' and prsano='".$_SESSION['exercicio']."' ";
   $db->executar($sql);
   $db->commit();
	   ?>
      <script>
         alert('Grava��o com sucesso.')
        // window.close();
      </script>
  <?
  //exit();
}
	
}


 if ($_REQUEST['tipo_obj']=='combo')
 {
 	if ($_REQUEST['campo']=='tprcod') 
 	$sql="select ".$_REQUEST['padrao']."nome as texto from ".$_REQUEST['alvo']." where ".$_REQUEST['campo']."=".$_REQUEST['especial'];
 	else $sql="select ".$_REQUEST['padrao']."dsc as texto from ".$_REQUEST['alvo']." where ".$_REQUEST['campo']."=".$_REQUEST['especial'];
 }
  else if ($_REQUEST['tipo_obj']=='bool')
 {
 	if ($_REQUEST['especial']=='f') $texto='N�o'; else $texto='Sim';
 }
 else  
 {
 	 $sql="select ".$_REQUEST['campo']." as texto from ".$_REQUEST['origem']." where acaid=".$_SESSION['acaid'];
 }
  if (! $texto) $texto = @$db->pegaUm($sql);
 if (! $texto) $texto='Sem valor atual';
  
  
 $sql="select '".$_REQUEST['campo']."' from '".$_REQUEST['origem']."' where prgid=".$_SESSION['prgid'];
 // print $sql;
  
    $sql="select p.eraid,p.erapropostacod, p.eraproposta,p.usucpf,p.erastatus,p.erastatusspo,erajustificativa  from elabrev.ElaboracaoRevisao p inner join seguranca.usuario u on u.usucpf='".$_SESSION['usucpf']."' and p.usucpf=u.usucpf   where p.eratabela='".$_REQUEST['origem']."' and p.eracod=".$_SESSION['acaid']."  and p.prsano='".$_SESSION['exercicio']."' and p.eracampo='".$_REQUEST['campo']."'";

    $RS = $db->record_set($sql);
    $nlinhas= $db->conta_linhas($RS);
    if ($nlinhas>=0)
    {
   		$res =  $db->carrega_registro($RS,0);
    	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    	$criadorpa = $db->ehcriador($eraid,'CP'); // criador da proposta ?
    }

    $sql="select acaid from elabrev.ppaacao_proposta where acaid=".$_SESSION['acaid']."  and prsano='".$_SESSION['exercicio']."' limit 1";

    $RS = $db->record_set($sql);
    $nlinhas= $db->conta_linhas($RS);
    if ($nlinhas>=0)
    {
       $res =  $db->carrega_registro($RS,0);
       if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    }
    
$momento = $db->verifica_momento();
$decisor = $db->ehdecisor($_SESSION['acaid'],'A',$momento); // verifica se � decisor
if ($decisor) $digit = $decisor;
else $digit = $db->ehdigitador($_SESSION['acaid'],'A',$momento); // verifica se � digitador
    
?>

<html>
<head>
<title>Proposta de Revis�o</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script language="JavaScript" src="../includes/funcoes.js"></script>
</head>
<body bgcolor="#ffffff" leftmargin="0" rightmargin="0" topmargin="0" bottommargin="0" marginheight="0" marginwidth="0">
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="origem" value="<?=$_REQUEST['origem']?>">
<input type=hidden name="alvo" value="<?=$_REQUEST['alvo']?>">
<input type=hidden name="campo" value="<?=$_REQUEST['campo']?>">
<input type=hidden name="decisao" value="0">
<input type=hidden name="cdecisao" value="0">
<input type=hidden name="grava" value="0">
<? if ($eraproposta) {?>
<input type=hidden name="inserir" value="2">
<?} else {?>
<input type=hidden name="inserir" value="1">
<?}

  $v_explicacao=$db->busca_explicacao($_REQUEST['campo'],$_REQUEST['origem']);
  $explicacao = $v_explicacao['explicacao'];
  $tmax=$v_explicacao['explicacao'];
  $tipo=$v_explicacao['tipo'];
?>
    <center>
    <table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
      <tr>
        <td align='right' class="subtitulodireita">A��o:</td>
        <td><?=$prgcod.'.'.$acacod.'-'.$acadsc?></td>
      </tr>
  
	  <tr>
        <td align="right" class="subtitulodireita">Atributo a ser alterado:</td> 
        <td colspan=2><b><?=$_REQUEST['nomcampo'];?></b></td>       
     </tr>  
	  <tr>
        <td align="right" class="subtitulodireita">Conte�do atual:</td> 
        <td colspan=2><?=$texto;?></td>       
     </tr>       
    
 	  <tr>
        <td align="right" class="subtitulodireita">Proposta:</td>
        <td><?
        if ($_REQUEST['tipo_obj']=='combo') 
        {
        	$sql = "select ".$_REQUEST['padrao']."cod as codigo, ".$_REQUEST['padrao']."dsc as descricao from ".$_REQUEST['alvo']." where ".$_REQUEST['padrao']."status='A' ";
        	if ($_REQUEST['especial']) $sql .= ' and '.$_REQUEST['padrao'].'cod not in ('.$_REQUEST['especial'].')';
        	if ($_REQUEST['especial2']) $sql .= ' and  ('.$_REQUEST['especial2'].')';
     	
        	$sql .= " order by ".$_REQUEST['padrao']."dsc";
        	$erapropostacod=trim($erapropostacod);
        $db->monta_combo('erapropostacod',$sql,'S','Escolha uma op��o','','',$explicacao,400);
		} 
        else 
        if ($_REQUEST['tipo_obj']=='text')
        {     
        	print campo_textarea('eraproposta','N','S','','80',5,'','','',$explicacao); 
        } 
        else 
        if ($_REQUEST['tipo_obj']=='texto')
        {
        	 print campo_texto('eraproposta','S','S','',80,$tmax,'','','', $explicacao,'');
        } 
        else 
        if ($_REQUEST['tipo_obj']=='numfrac'){
        	print campo_texto('eraproposta','S','S','',12,$tmax,'##########.##','','', $explicacao,'');
        }  
        else 
        if ($_REQUEST['tipo_obj']=='numint'){
            print campo_texto('eraproposta','S','S','',10,$tmax,'##########','','', $explicacao,'');
            }
        if ($_REQUEST['tipo_obj']=='data'){
        	print '<script language="JavaScript" src="../includes/calendario.js"></script>';
            print campo_data('eraproposta','S','S','','S','','','','', $explicacao,'');  
            }            
        ?> </td>

     </tr>
	  <tr>
        <td align="right" class="subtitulodireita">Justificativa:</td> 
        <td ><?=campo_textarea('erajustificativa','S','S','','80',5,'');?></td>          </tr>
	 <tr>
	 <td colspan="2" align="right" class="subtitulodireita">
	 <?
	 if ($momento and $digit and $erastatus<>'D' and $erastatusspo<>'S')
	 {?>
	<input type='button' class="botao" value='OK!' onclick="grava_proposta()">&nbsp;&nbsp;&nbsp;
<?}?> <input type='button' class="botao" value='Fechar' onclick="fechar_janela()"></td>
	 </tr>
  </table>
</form> 

<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
      <td valign="top" class="title"><strong>A��o</strong></td>
      <td valign="top" class="title"><strong>Autor</strong></td>
      <td valign="top" class="title"><strong>Proposta</strong></td>
    </tr>
</thead>
<tbody>
<?
$sql="select p.eraid,p.erastatus,p.erastatusspo, p.usucpf,p.usucpfdecisor,p.usucpfdecisorspo, '<img border=\"0\" src=\"../imagens/IconeAjuda.gif\" title=\" Ver justificativa \" onclick=\"ver_justificativa(' || p.eraid|| ')\">' as verjustificativa,p.eraproposta ||'<br> Digitado em: '||to_char(p.eradata,'dd/mm/yyyy') as proposta,
 case when u.usucpf !='".$_SESSION['usucpf']."'  then '<img border=\"0\" src=\"../imagens/email.gif\" title=\" Enviar E-mail \" onclick=\"envia_email(' || '\'' || u.usucpf || '\'' || ')\">&nbsp;'||u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum else u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum end as autor, case when p.usucpfdecisor is not null then u2.usunome else '' end as decisoruo, case when p.usucpfdecisorspo is not null then u3.usunome else '' end as decisorspo from elabrev.ElaboracaoRevisao p inner join seguranca.usuario u on p.usucpf=u.usucpf left join seguranca.usuario u2 on u2.usucpf=p.usucpfdecisor left join seguranca.usuario u3 on u3.usucpf=p.usucpfdecisorspo where p.erastatus in ('A','D') and  p.eratabela='".$_REQUEST['origem']."' and p.eracod=".$_SESSION['acaid']." and p.prsano='".$_SESSION['exercicio']."' and p.eracampo='".$_REQUEST['campo']."' order by p.erastatus desc,p.eradata desc";

$RS2 = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS2);
if ($nlinhas >= 0) {
for ($i=0; $i<=$nlinhas;$i++){
    $res = $db->carrega_registro($RS2,$i);
    // a linha abaixo transforma em vari�veis todos os campos do array
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    $criadorpa = $db->ehcriador($eraid,'CP'); // criador da proposta ?
	if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
    $decisao='';
		if ($momento==1)
	    {
			// ent�o est� dentro do momento
			// verifico se j� possui decis�o
			if ($erastatus=='D')
			{
				// ent�o algu�m j� decidiu
				// verifico se o decisor � o mesmo que est� logado. se for, ent�o pode cancelar a decis�o
				if ($usucpfdecisor==$_SESSION['usucpf'] or $db->testa_superuser())
				{
					// ent�o tem pleno acesso
					$decisao="<img border='0' src='../imagens/valida1.gif' title=' Cancelar a decis�o!' onclick=\"cancelar_decisao('".$eraid."')\">";
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
					$decisao="<img border='0' src='../imagens/valida2.gif' title=' Decidir por esta proposta!' onclick=\"tomar_decisao('".$eraid."')\">";
				}
			}
		}
		if ($momento==2)
	    {
			// ent�o est� dentro do momento SPO
			// verifico se j� possui decis�o
			if ($erastatus=='D')
			{
				// ent�o algu�m j� decidiu
				$decisao2="<img border='0' src='../imagens/valida1.gif' title=' Proposta j� decidida n�vel UO por $decisoruo!' >";            
			} else $decisao2='';
			if ($erastatusspo=='S')
			{
				if ($db->ehcoorduma($usucpfdecisorspo)) $valid='valida6.gif' ; else $valid='valida4.gif';
				// ent�o algu�m j� decidiu
				// verifico se o decisor � o mesmo que est� logado. se for, ent�o pode cancelar a decis�o
				if ($usucpfdecisorspo==$_SESSION['usucpf'] or $db->testa_superuser())
				{
					// ent�o tem pleno acesso
					$decisao3="<img border='0' src='../imagens/$valid' title=' Cancelar a decis�o n�vel SPO!' onclick=\"cancelar_decisao('".$eraid."')\">";
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
					$decisao3="<img border='0' src='../imagens/valida5.gif' title=' Decidir por esta proposta n�vel SPO!' onclick=\"tomar_decisao('".$eraid."')\">";
				}
			}
		}		
	print '<tr class="'.$marcado.'"><td>'.$verjustificativa.'&nbsp;&nbsp;'.$decisao.$decisao2.$decisao3.'</td><td>'.$autor.'</td><td>'.$proposta.'</td></tr>';

  }
}
else
{
  print '<tr class="'.$marcado.'"><td></td><td>N�o h� registros de propostas de altera��o</td></tr>';
}
?>
</tbody>
    </table>



<script>
  function fechar_janela()
  {
    window.close();

  }
  function grava_proposta()
  {
  	if (!validaBranco(document.formulario.erajustificativa, 'Justificativa')) return;
   	try
	{
   	if (!validaBranco(document.formulario.erapropostacod, 'Proposta')) return;
	} 
	catch(e)
	{
		if (!validaBranco(document.formulario.eraproposta, 'Proposta')) return; 
	}
  	document.formulario.grava.value = 1;
	document.formulario.submit();
  }

  function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }

      function ver_justificativa(cod)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/ver_justificativa&acao=A&cod="+cod;          
          window.open(e, "Justificativa","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }    

     function tomar_decisao(cod)
    {
    	if( window.confirm( "Confirma a decis�o?") )
         {
	         document.formulario.decisao.value = cod;
	         document.formulario.submit();
         } else return;
    }  
      
     function cancelar_decisao(cod)
    {
    	if( window.confirm( "Confirma o CANCELAMENTO da decis�o?") )
         {
	         document.formulario.cdecisao.value = cod;
	         document.formulario.submit();
         } else return;
    }
        
</script>
</body>
</html>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script> 
