<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   M�dulo:prn_prop_aca.inc
   Finalidade: permitir a constru��o de relatorio de propostas em a��o
    */
/*
session_cache_limiter('private_no_expire');
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
header("Cache-Control: no-cache");
header("Pragma: no-cache");
session_start(); */


define('FPDF_FONTPATH','includes/fpdf/font/');
require(APPRAIZ.'includes/fpdf/fpdf.php');

class PDF extends FPDF
{
//Page header
function Header()
{
    global $title;
    global $title2;
    global $title3;
    global $title4;
    global $title5;
    global $title6;
    global $title7;
	global $header;
	global $w;
	// autor
	$this->SetAuthor('Sistema Integrado de Monitoramento do Minist�rio da Educa��o');
	$this->SetSubject('Relat�rio Padronizado de Propostas em a��o');
	$this->SetTitle($title2);
    //Logo
    $this->Image('/var/www/html/simec/www/imagens/logo_mec_br.jpg',40,20,525);
    $this->Image('/var/www/html/simec/www/imagens/top.jpg',40,43,525);
    $this->Image('/var/www/html/simec/www/imagens/livros_mec.jpg',190,380,200);
   // $this->Image('/var/home/simec/sistemas/www/imagens/logo_mec_br.jpg',40,20,525);
   // $this->Image('/var/home/simec/sistemas/www/imagens/top.jpg',40,43,525);
   // $this->Image('/var/home/simec/sistemas/www/imagens/livros_mec.jpg',190,380,200);
    $this->Ln(70);
  
    //$this->Ln(20);
    //Arial bold 15
    $this->SetFont('Arial','B',10);
    //Move to the right
   // $this->Cell(80);
    //Title
    $this->SetLeftMargin(40) ;
    if ($title <> "")  $this->Cell(0,30,$title,0,1,'C');
    if ($title2 <> "") $this->Cell(0,15,$title2,0,1,'C');
    if ($title3 <> "") $this->Cell(0,15,$title3,0,1,'C');
    if ($title4 <> "") $this->Cell(0,15,$title4,0,1,'C');
    if ($title5 <> "") $this->Cell(0,15,$title5,0,1,'C');
    if ($title6 <> "") $this->Cell(0,15,$title6,0,1,'C');
    if ($title7 <> "") $this->Cell(0,15,$title7,0,1,'C');
    $this->Ln(20);
	   for($i=0;$i<count($header[0]);$i++)
        $this->Cell($w[$i],15,$header[0][$i],1,0,'C');
    $this->Ln();
}

// function Rotate
        function Rotate($angle,$x=-1,$y=-1) {
                if($x==-1)
                        $x=$this->x;
                if($y==-1)
                        $y=$this->y;
                if($this->angle!=0)
                        $this->_out('Q');
                $this->angle=$angle;
                if($angle!=0) {
                        $angle*=M_PI/180;
                        $c=cos($angle);
                        $s=sin($angle);
                        $cx=$x*$this->k;
                        $cy=($this->h-$y)*$this->k;
                        $this->_out(sprintf('q %.5f %.5f %.5f %.5f %.2f %.2f cm 1 0 0 1 %.2f %.2f cm',$c,$s,-$s,$c,$cx,$cy,-$cx,-$cy));
                }
        }

// function RotatedText
        function RotatedText($x,$y,$txt,$angle) {
                //Text rotated around its origin
                $this->Rotate($angle,$x,$y);
                $this->Text($x,$y,$txt);
                $this->Rotate(0);
        }

//Page footer
function Footer()
{
    global $dat;
    global $title3;

    //Position at 1.5 cm from bottom
    $this->SetY(-95);
    $this->setX(40);
    $this->SetFont('Arial','I',6);
    $j=-95;
    foreach ($_SESSION['filtrorelat'] as $filtro)
    {
       $j = $j+6;
       $this->write(8,$filtro);
       $this->SetY($j);
       $this->setX(40);

    }

    $this->SetY(-35);
    $this->setX(40);
    $this->Write(6,'Data: '.date('d/m/Y'));
    $this->setX(300);
    $this->Write(6,'P�gina '.$this->PageNo(),'',$this->getY()-12);

}

//Simple table
function PrnWrite($tit,$dado,$y)
{
    $yy = $this->getY();
    if ($y == $yy) $dir=12; else $dir = 0;
    $this->setxy(40,$this->gety()+$dir);
    //$this->SetFont('Arial','B',8);
    $this->write(12,$tit);
    //$this->SetFont('Arial','',8);
    $this->SetLeftMargin(120);
    $y = $this->getY();
    $this->write(12,$dado);
    $y = $this->getY();
    return $y;
}


function Exclusao($acaid,$i)
{
    global $db;
    global $linha;

    $this->SetFont('Arial','',8);
    $this->setxy(40,150);
    $this->setxy(120,150);
    $yp=$this->PrnWrite('PROPOSTAS DE EXCLUS�O: ','',150);
    $yp=$this->PrnWrite('A��o: ',$linha['prgcod'].'-'.$linha['acacod'].'-'.$linha['acadsc'],$yp); 
   foreach ($_SESSION['rsexclusao'][$i] as $dados)
   {
   	  if ($dados['acao'] == $acaid)
   	  {
   	  	// imprimo
   	  	$yp=$this->PrnWrite('Proponente: ',$dados['proponente'],$yp);
        $yp=$this->PrnWrite('Justificativa: ',trim($dados['justificativa']),$yp);                  
        $yp=$this->PrnWrite('Status de decis�o:',trim($dados['status']),$yp); 
        $yp=$this->PrnWrite('Decisor:',$dados['decisor'],$yp);
        $yp=$this->PrnWrite('','',$yp);
       // $this->Image('/var/home/simec/sistemas/www/imagens/linha.jpg',40,$yp,525);  
		$this->Image('/var/www/html/simec/www/imagens/linha.jpg',40,$yp,525);
   	  	
   	  }
   }
      
 }
 
 
function Migracao($acaid,$i)
{
    global $db;
    global $linha;

    $this->SetFont('Arial','',8);
    $this->setxy(40,150);
    $this->setxy(120,150);
    $yp=$this->PrnWrite('PROPOSTAS DE MIGRA��O: ','',150);
    $yp=$this->PrnWrite('A��o: ',$linha['prgcod'].'-'.$linha['acacod'].'-'.$linha['acadsc'],$yp); 

   foreach ($_SESSION['rsmigracao'][$i] as $dados)
   {
   	  if ($dados['acao'] == $acaid)
   	  {
   	  	// imprimo
   	  	$yp=$this->PrnWrite('Proponente: ',$dados['proponente'],$yp);
        $yp=$this->PrnWrite('Migrar para: ',trim($dados['destino']),$yp);         
   	  	$yp=$this->PrnWrite('Justificativa: ',trim($dados['justificativa']),$yp);                  
        $yp=$this->PrnWrite('Status de decis�o:',trim($dados['status']),$yp); 
        $yp=$this->PrnWrite('Decisor:',$dados['decisor'],$yp);
        $yp=$this->PrnWrite('','',$yp);
       //$this->Image('/var/home/simec/sistemas/www/imagens/linha.jpg',40,$yp,525); 
		$this->Image('/var/www/html/simec/www/imagens/linha.jpg',40,$yp,525);
   	  	
   	  }
   }    
       
} 
 
function Fusao($acaid,$i)
{
    global $db;
    global $linha;

    $this->SetFont('Arial','',8);
    $this->setxy(40,150);
    $this->setxy(120,150);
    $yp=$this->PrnWrite('PROPOSTAS DE FUS�O: ','',150);
    $yp=$this->PrnWrite('A��o: ',$linha['prgcod'].'-'.$linha['acacod'].'-'.$linha['acadsc'],$yp); 

   foreach ($_SESSION['rsfusao'][$i] as $dados)
   {
   	  if ($dados['acao'] == $acaid)
   	  {
   	  	// imprimo
   	  	$yp=$this->PrnWrite('Proponente: ',$dados['proponente'],$yp);
        $yp=$this->PrnWrite('Funde com: ',trim($dados['destino']),$yp);         
   	  	$yp=$this->PrnWrite('Justificativa: ',trim($dados['justificativa']),$yp);                  
        $yp=$this->PrnWrite('Status de decis�o:',trim($dados['status']),$yp); 
        $yp=$this->PrnWrite('Decisor:',$dados['decisor'],$yp);
        $yp=$this->PrnWrite('','',$yp);
       // $this->Image('/var/home/simec/sistemas/www/imagens/linha.jpg',40,$yp,525); 
		$this->Image('/var/www/html/simec/www/imagens/linha.jpg',40,$yp,525);
   	  	
   	  }
   }     
      
 }  

function Alt_Atributo($acaid,$i)
{
    global $db;
    global $linha;

    $this->SetFont('Arial','',8);
    $this->setxy(40,150);
    $this->setxy(120,150);
    $yp=$this->PrnWrite('PROPOSTAS DE ALTERA��O EM ATRIBUTOS: ','',150);
    $yp=$this->PrnWrite('A��o: ',$linha['prgcod'].'-'.$linha['acacod'].'-'.$linha['acadsc'],$yp); 

   foreach ($_SESSION['rsalteracao'][$i] as $dados)
   {
   	  if ($dados['acao'] == $acaid)
   	  {
   	  	// imprimo
   	  	$yp=$this->PrnWrite('Proponente: ',$dados['proponente'],$yp);
        $yp=$this->PrnWrite('Atributo: ',trim(tradutor_att($dados['atributo'])),$yp);         
   	  	$yp=$this->PrnWrite('Proposta: ',trim($dados['proposta']),$yp); 
        $yp=$this->PrnWrite('Justificativa: ',trim($dados['justificativa']),$yp);                  
        $yp=$this->PrnWrite('Status de decis�o:',trim($dados['status']),$yp); 
        $yp=$this->PrnWrite('Decisor:',$dados['decisor'],$yp);
        $yp=$this->PrnWrite('','',$yp);
       // $this->Image('/var/home/simec/sistemas/www/imagens/linha.jpg',40,$yp,525);  
		$this->Image('/var/www/html/simec/www/imagens/linha.jpg',40,$yp,525);
   	  	
   	  }
   }
       
 }  
}
  
//Instanciation of inherited class

$pdf=new PDF('P','pt','A4');
$pdf->SetFont('Times','',6);

$title = 'SISTEMA INTEGRADO DE MONITORAMENTO DO MINIST�RIO DA EDUCA��O';
$title2 = 'M�dulo de Elabora��o e Revis�o do PPA';
$title3 = 'Proposta de Altera��o em A��es';

$pdf->Open();
//dbg($_SESSION['rsexclusao'][0][0]['proponente'],1);
if (count($_SESSION['acaoidrelat']) > 0)
{
  for ($i=0; $i<=count($_SESSION['acaoidrelat'])-1;$i++)
  {
   $acaid = $_SESSION['acaoidrelat'][$i];
   $sql="select distinct prgcod, acacod, acadsc from elabrev.ppaacao_proposta where acaid=$acaid";
   $linha=$db->pegalinha($sql,0);
   
   if ($_SESSION['rsexclusao'][$i][0]['proponente']  <>'') {$pdf->AddPage();$pdf->Exclusao($acaid,$i);}
   if ($_SESSION['rsmigracao'][$i][0]['proponente']  <>'')  {$pdf->AddPage();$pdf->Migracao($acaid,$i);}
   if ($_SESSION['rsfusao'][$i][0]['proponente']  <>'') {$pdf->AddPage();$pdf->Fusao($acaid,$i);} 
   if ($_SESSION['rsalteracao'][$i][0]['proponente']  <>'')  {$pdf->AddPage();$pdf->Alt_Atributo($acaid,$i);}     
  }
}

$pdf->Output();
?>
