<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   M�dulo:cadacao.inc
   Finalidade: permitir o controle do cadastro de acao
   */
$modulo=$_REQUEST['modulo'] ;//


if ($_REQUEST['prgid']) $_SESSION['prgid'] = $_REQUEST['prgid'];
else $_REQUEST['prgid'] =$_SESSION['prgid'];

if ($_REQUEST['acaid']) $_SESSION['acaid']= $_REQUEST['acaid']; 
else $_REQUEST['acaid'] = $_SESSION['acaid'];


$sql = "select a.*, b.prgdsc from ".$_SESSION['sisdiretorio'].".ppaacao_proposta a, ".$_SESSION['sisdiretorio'].".ppaprograma_proposta b where a.acastatus in ('A','N') and b.prgstatus='A' and a.prgid = b.prgid and a.prsano ='" .$_SESSION['exercicio']."' and a.prgid=".$_SESSION['prgid'];

if ($_REQUEST['acacod'] )
{
    $sql = $sql. "  and a.acacod='".$_REQUEST['acacod']."' limit 1";
}
elseif ($_SESSION['acaid'])
{
	$sql = $sql. "  and a.acaid=".$_SESSION['acaid'];
}

$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);

$nl=$nlinhas;

if ($nlinhas >= 0) {
    $res =  $db->carrega_registro($RS,$_POST['registro']);
    if(is_array($res)){
       foreach($res as $k=>$v) {${$k}=$v;};
            $_SESSION['prgid'] = $prgid;
            $_SESSION['acaid']= $acaid;

            if ($acasnmetanaocumulativa=='t') $tipometa ='N�o cumulativa';
            if ($acasnmetanaocumulativa=='f') $tipometa ='Cumulativa';
            if ($acasnmetanaocumulativa=='') $tipometa ='N�o informado';
       }
	// encontrar o desafio (macro-objetivo)
        $sql= "select prodsc from produto where procod='".$procod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar descrição da unidade de medida
        $sql= "select unmdsc from unidademedida where unmcod='".$unmcod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar descrição do tipo de ação
        $sql= "select tacdsc from tipoacao where taccod='".$taccod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar a função
       if ($funcod) {
        $sql= "select fundsc from elabrev.ppafuncao where funcod='".$funcod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        }
        // encontrar a subfunção
       if ($sfucod) {
        $sql= "select sfudsc from elabrev.ppasubfuncao where sfucod='".$sfucod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
       }
    // encontrar o órgão responsável
        $sql= "select orgdsc from elabrev.ppaorgao where orgcod='".$orgcod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

    // encontrar o orgao responsável
        $sql= "select orgdsc from elabrev.ppaorgao where substr(orgcod,0,3)='".substr($unicod,0,2)."'";

        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        
    // encontrar a unidade responsável
        $sql= "select unicod||'-'||unidsc as unidsc from unidade where unicod='".$unicod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    // encontrar o produto
        $sql= "select prodsc from produto where procod='".$procod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    // encontrar a meta
        //$sql= "select fisqtdeprevistoano as meta from elabrev.dadofisico where acaid='$acaid' and prsano='".$_SESSION['exercicio']."'";
       //$res = $db->recuperar($sql);
       // if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    // encontrar a esfera
       if ($esfcod) {
        $sql= "select esfdsc from esfera where esfcod='".$esfcod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        }
 } else
 {
   ?>
<script>
    alert('C�digo da A��o inexistente.! \nVoc� pode estar tentando abrir a��o de outro exerc�cio.');
    history.back();
</script>
   <?
   exit();
 }

include APPRAIZ."includes/cabecalho.inc";		
print "<br>";
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Altera��o dos Atributos da A��o/Fus�o de A��es';
monta_titulo($titulo_modulo,'');
?>
<? // inclus�o do menu de setas de navega��o
$nlinhas=$nl;
?> 
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="orgao" value=0>
<input type=hidden name="registro" value=<?=$_POST['registro'];?>>
<input type=hidden name="navega" value=0>
<input type=hidden name="procura" value=0>

	 <tr align="left">
        <td align='left' class="SubTituloDireita">Programa:</td>
        <td><b><?=$prgcod?></b> - <?=$prgdsc?>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita"></td>
        <td>
        <input type="button" name="btnAbrirResp" TITLE="Se esta a��o est� recebendo dados de outra (Fus�o), clique aqui para indicar." value="Fus�o" onclick="popfun('<?=$acacod?>','<?=$prgcod?>')"></td>
      </tr>        
      <tr>
        <td align='right' class="SubTituloDireita">Fun��o:</td>
        <td><?=campo_texto('fundsc','N','N','',50,50,'','','','dica',"elabrev.ppafuncao;funcod;fun;combo;ppaacao_proposta;Fun��o;$funcod");?></td>
     </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Subfun��o:</td>
        <td><?=campo_texto('sfudsc','N','N','',50,50,'','','','dica',"elabrev.ppasubfuncao;sfucod;sfu;combo;ppaacao_proposta;Subfun��o;$sfucod");?></td>      
      </tr>     
      <tr>
        <td align='right' class="SubTituloDireita">T�tulo:</td>
        <td><b><?=$acacod?></b>.<?=campo_textarea('acadsc','N','N','',50,3,'','dica','');?></td>
      </tr>
     <tr>
        <td align='right' class="SubTituloDireita">Finalidade:</td>
        <td><?=campo_textarea('acafinalidade','N','N','',50,5,'','dica','ppaacao_proposta;acafinalidade;aca;text;ppaacao_proposta;Finalidade');?></td>         
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Descri��o:</td>  
        <td><?=campo_textarea('acadescricao','N','N','',50,5,'','dica','ppaacao_proposta;acadescricao;aca;text;ppaacao_proposta;Descricao');?></td>          
      </tr>
       <tr>
        <td align='right' class="SubTituloDireita">Produto (Bem ou Servi�o):</td>
        <td><?=campo_texto('prodsc','N','N','',50,50,'','','','dica',"produto;procod;pro;combo;ppaacao_proposta;Produto;$procod");?></td> 
      </tr>
      <!-- <tr>
        <td align='right' class="SubTituloDireita">Meta (<b><?//=//$tipometa?>)</b>:</td>
        <td><?//=//campo_texto('acaqtdprevistoanocorrente','N','N','',50,50,'','');?></td>
      </tr>-->
      <tr>
        <td align='right' class="SubTituloDireita">Especifica��o do Produto:</td>
        <td><?=campo_textarea('acadscproduto','N','N','',50,3,'','dica','ppaacao_proposta;acadscproduto;aca;text;ppaacao_proposta;Especifica��o do produto');?></td>          
      </tr>      
      <tr>
        <td align='right' class="SubTituloDireita">Unidade de Medida:</td>
        <td><?=campo_texto('unmdsc','N','N','',50,50,'','','','dica',"unidademedida;unmcod;unm;combo;ppaacao_proposta;Unidade de Medida;$unmcod");?></td> 
      </tr>
      <tr>
	     <td align='right' class="SubTituloDireita">Tipo de a��o:</td>
        <td><?=campo_texto('tacdsc','N','N','',50,50,'','','','dica',"tipoacao;taccod;tac;combo;ppaacao_proposta;Tipo de a��o;$taccod");?></td> 
      </tr>
        <tr>
	      <td align='right' class="SubTituloDireita">Esfera:</td>
        <td><?=campo_texto('esfdsc','N','N','',50,50,'','','','dica',"esfera;esfcod;esf;combo;ppaacao_proposta;Esfera;$esfcod");?></td>           
         </tr> 
              
      <tr>
	     <td align='right' class="SubTituloDireita">Tipo de Or�amento:</td>
        <td><?//=campo_texto('tacdsc','N','N','',50,50,'','','','dica','tipoacao;taccod;tac;combo;ppaacao_proposta;Tipo de a��o');?></td> 
      </tr> 
      <tr>
	     <td align='right' class="SubTituloDireita">Forma de Implementa��o:</td>

         <td>
         <table>
           <tr>
             <td >
                Direta<input type="checkbox" value="t" name="acasnmedireta" <?if ($acasnmedireta=='t') print '   checked'?>>
            </td>
            <td>Descentralizada<input type="checkbox" value="t" name="acasnmedesc" <?if ($acasnmedesc=='t') print '   checked'?>>
            </td>
            <td colspan="2">
               Linha de Cr�dito<input type="checkbox" value="t" name="acasnmelincred" <?if ($acasnmelincred=='t') print '   checked'?>>
            </td>
         </tr>       
         <tr>
            <td>
                Transfer�ncia<input type="checkbox" value="T" name="lista" <?if ($lista=='T') print '   checked'?>>                  
            </td>
           
            <td>Obrigat�ria<input type="checkbox" value="t" name="acasntransfobrigatoria" <?if ($acasntransfobrigatoria=='t') print '   checked'?>>
            </td>
            <td>Volunt�ria<input type="checkbox" value="t" name="acasntransfvoluntaria" <?if ($acasntransfvoluntaria=='t') print '   checked'?>>
            </td>
            <td>Outras<input type="checkbox" value="t" name="acasntransfoutras" <?if ($acasntransfoutras=='t') print '   checked'?>>            
            </td>
         </tr>
         </table>         
         </td>
      </tr>      
      <tr>
        <td align='right' class="SubTituloDireita">Detalhamento da Implementa��o:</td>
        <td><?=campo_textarea('acadetalhamento','N','N','',50,5,'','dica','ppaacao_proposta;acadetalhamento;aca;text;ppaacao_proposta;Detalhamento da implementa��o');?></td>         
      </tr>      
      <tr>
        <td align='right' class="SubTituloDireita">Base legal:</td>
        <td><?=campo_textarea('acabaselegal','N','N','',50,5,'','dica','ppaacao_proposta;acabaselegal;aca;text;ppaacao_proposta;Base legal');?></td>          
	</td>
      </tr> 
     
     <tr>
        <td align='right' class="SubTituloDireita">Observa��es:</td>
        <td><?=campo_textarea('acaobs','N','N','',50,5,'','dica','ppaacao_proposta;acaobs;aca;text;ppaacao_proposta;Observa��o');?></td>         
      </tr>   
      
  
  <?   
  $sql="select distinct ur.usucpf,pfl.pflcod as codigo, pfl.pfldsc as descricao,
pfl.pflsncumulativo,usu.usunome || '('|| org.orgdsc ||')' as usuario
from seguranca.perfil pfl
inner join elabrev.tprperfil tf on tf.pflcod=pfl.pflcod
inner join elabrev.tiporesponsabilidade tr on tr.tprcod=tf.tprcod and tr.tprsigla in ('U')
left join seguranca.perfilusuario pu on pu.pflcod=tf.pflcod
left join seguranca.usuario usu on usu.usucpf=pu.usucpf
inner join orgao org on org.orgcod=usu.orgcod
left join elabrev.usuarioresponsabilidade ur on ur.usucpf=pu.usucpf and ur.rpustatus='A'
inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod and ua.acaid=".$_SESSION['acaid']."
where pfl.pflstatus='A' and pfl.sisid=".$_SESSION['sisid']." and ur.rpustatus='A'";

  //print $sql;
    /*$RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($i=0; $i<=$nlinhas;$i++){
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em vari�veis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	        $linha = "<tr><td align='right' class='SubTituloDireita'><b>".$descricao.":</b></td><td>".$usuario."</td></tr>";
            print $linha;
        }
     }*/?>
    </table>
 </form>  

<script>
   function edita_campo(tabela_alvo,campo,padrao,tipo_obj,tabela_origem,nome_campo,especial,especial2)
   {
   	 e = '<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/propostarevisao&acao=A&alvo='+tabela_alvo+'&campo='+campo+'&padrao='+padrao+'&tipo_obj='+tipo_obj+'&origem='+tabela_origem+'&nomcampo='+nome_campo+'&especial='+especial+'&especial2='+especial2;
      window.open(e, "Associa��o_de_Respons�veis","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,left=0,top=0,width=600,height=450");
     // &acao=A&alvo=orgao&campo=orgcod&padrao=org&tipo_obj=combo,acao&tabela_origem=undefined
   }
  function popfun(acacod,prgcod)
  {
	e = '<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/propostafusao&acao=A';
		window.open(e, "Associa��o_de_Respons�veis","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,left=0,top=0,width=500,height=450");
  }
    function Associa_resp()
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/ppafasequalitativa/acao/assocrespac&acao=A";
          WindowObjectReference = window.open(e, "Associa��o_de_Respons�veis",
"menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=600,height=400'");
    }  
</script>

