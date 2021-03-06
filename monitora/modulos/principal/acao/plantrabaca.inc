<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   M�dulo:plantrabaca.inc
   Finalidade: permitir o controle de cadastro de atividades num plano de trabalho de a��o
   */
$modulo=$_REQUEST['modulo'] ;//
//print $modulo;


function preparaCamposDaAcao( &$acacod , &$acadsc, &$etapafase, &$et )
{
	global $db;
	
	$sql =	"
			SELECT
				acaid , taccod , acasnrap , acacod , acadsc
			FROM
				monitora.acao
			WHERE
				acaid = " . $_SESSION['acaid'] .
			"";
			
	$registro=$db->recuperar($sql);
	
	if	( $registro['acansrap'] )
	{
		?>
			<script>alert('Esta a��o � de Restos a Pagar e n�o � monitorada!');
			history.back();</script>
		<?
		$db->close();
		exit();
	}
	
	if	( $registro['taccod'] == 1 )
	{
		$etapafase = 'Etapa';
		$et='E';
	}
	else
	{
		$etapafase='Fase';
		$et='F';
	}
	
	$acacod = $registro['acacod'];
	$acadsc = $registro['acadsc'];
}

$acacod = '';
$acadsc = '';
$etapafase = '';
$et = '';
preparaCamposDaAcao( $acacod , $acadsc , $etapafase , $et );

include APPRAIZ."includes/cabecalho.inc";
?>
	<script language="JavaScript" src="../includes/calendario.js"></script>
	<br/>
	<?
		$db->cria_aba($abacod_tela,$url,'');
		$titulo_modulo='Estruturar o Plano de Trabalho da A��o';
		monta_titulo($titulo_modulo,'');
	?>
	<div align="center">
		<center>
		<?
			// verifica se � coordenador de a��o
			$autoriza = false;
			$coordaca=false;
			$digit=false;
			
			if	( $db->testa_coordenador( $_SESSION['acaid'] , 'A' ) )
			{
				$coordaca = true;
			}

			// verifica se � digitador
			if	( $db->testa_digitador( $_SESSION['acaid'] , 'A' ) )
			{
				$digit = true;
			}
			
			// verific se � super-usu�rio
			if	( $db->testa_superuser() )
			{
				$coordaca = true;
			}

			// verifica se o exerc�cio est� aberto para estruturar o simec
  			$sql=	" SELECT 
						prsano 
					FROM
						monitora.programacaoexercicio
					WHERE
						prsano = '".$_SESSION['exercicio']."'
					AND
						prsstatus='A'
					";

			$registro=$db->pegaUm($sql);

			if ($registro)
			{
				$autoriza = true;
			}
			else
			{ 
    			// n�o est� autorizado, ent�o verifica se h� alguma autoriza��o especial
				$sql= "select ae.aelid from autorizacaoespecial ae where ae.acaid =".$_SESSION['acaid']." and ae.aelstatus ='A' and ae.aeldata_inicio <='".date('Y-m-d')."' and ae.aeldata_fim >='".date('Y-m-d')."' and ae.togcod=8 ";
				$registro=$db->pegaUm($sql);
				
				if ( $registro )
				{
					$autoriza = true;
				}
			}
			if ( ! $coordaca and ! $digit and ! $db->testa_coorduma() and ! $db->testa_uma())
			{
			    ?>
			       <script>
			          alert ('Voc� n�o tem acesso para monitorar o Plano de Trabalho nesta A��o!');
			          history.back();
			       </script>
			    <?
			    exit();
			}
		?>
	</center>
</div>
<?
$acaoId = $_SESSION['acaid'];
define( "APP_PLANO_TRABALHO_ACAO" , APPRAIZ . "includes/planodetrabalho/tarefa_acao/" );
require_once( APP_PLANO_TRABALHO_ACAO . 'arquivos_requeridosAcao.inc' );
require_once( APP_PLANO_TRABALHO_ACAO . 'lista_tarefas_da_acaoAcao.inc' );
?>