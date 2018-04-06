<?
/*
 * Variaveis a serem setadas na SESS�O DO SCRIPT
 * 
 *  SETAR : $_SESSION['downloadfileszip']['tipobuscararquivos'] : Define o tipo de busca que ser� realizado, o usu�rio pode implementar algum tipo caso n�o tenha.
 *  $$ OP��ES CADASTRADAS $$
 *  'buscarporsql' => Significa que a busca ser� pela tabela de public.arquivo (procedimento padr�o de grava��o no SIMEC). � necessario os campos 'arqid' e 'arqextensao'
 *  $$$ PARAM�TROS RECEBIDOS $$$
 * 	'crtnum' => Indica qual SQL dever� ser executado, caso n�o exista, ser� executado todos os SQL cadastrados no array ($_SESSION['downloadfileszip']['bd']).
 *  SETAR : $_SESSION['downloadfileszip']['pasta'] : � um array contendo as pasta de origem e destino do arquivos tratados. Devera conter o seguinte array : array("origem"=>"yyyyyy","destino"=>"xxxxxx")
 * 
 *   
 */

session_start();

// carrega as fun��es gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

# Verifica se a sess�o n�o expirou, se tiver expirada envia pra tela de login.
controlarAcessoSemAutenticacao();

// abre conex�o com o servidor de banco de dados
$db = new cls_banco();

function deletararquivos($files) {
	foreach($files as $fl) {
		unlink($fl);
	}
}

function processararquivos($files,$orig,$dest) {
	if($files[0]) {
		foreach($files as $f) {
			$endorigem =  "../../arquivos/".$orig."/".floor($f['arqid']/1000)."/".$f['arqid'];
			$enddestino = "../../arquivos/".$dest."/files_tmp/".$f['arqid'].".".$f['arqextensao'];
			if(file_exists($endorigem)) {
				if(copy($endorigem, $enddestino)) {
					$fzip[] = $enddestino; 	
				}
			}
		}
	}
	return $fzip;
}

include('../../includes/pclzip-2-6/pclzip.lib.php');
switch($_SESSION['downloadfileszip']['tipobuscararquivos']) {
	case 'buscarporlistaid':
		if(count($_REQUEST['fotosselecionadas']) > 0) {
			$files = $db->carregar("SELECT arqid, arqextensao FROM public.arquivo WHERE arqid IN('".implode("','",$_REQUEST['fotosselecionadas'])."')");
			$filezip = processararquivos($files,$_SESSION['downloadfileszip']['pasta']['origem'],$_SESSION['downloadfileszip']['pasta']['destino']);
			$nomearquivozip = $_SESSION['usucpf'].'_'.date('dmyhis').'.zip';
			$enderecozip = '../../arquivos/'.$_SESSION['downloadfileszip']['pasta']['destino'].'/files_tmp/'.$nomearquivozip;
			$archive = new PclZip($enderecozip);
			$archive->create( $filezip,  PCLZIP_OPT_REMOVE_ALL_PATH);
			deletararquivos($filezip);
		} else {
			echo "<script>alert('N�o foi selecionado nenhuma foto.');window.close();</script>";
			exit;
		}
		header("Content-Disposition: attachment; filename=\"$nomearquivozip\"");
		header("Content-Type: application/oct-stream");
		header("Expires: 0");
		header("Pragma: public");
		header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
		readfile($enderecozip);
		break;
}
?>