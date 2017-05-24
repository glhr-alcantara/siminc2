﻿BEGIN; --ROLLBACK; COMMIT;

ALTER TABLE monitora.pi_niveletapaensino
   ADD COLUMN mdeid integer NOT NULL DEFAULT NULL;
COMMENT ON COLUMN monitora.pi_niveletapaensino.mdeid
  IS 'Relação com a tabela de Área Cultural. monitora.pi_modalidadeensino';

ALTER TABLE monitora.pi_niveletapaensino
   ADD CONSTRAINT fk_pi_niveletapaensino_reference_pi_modalidadeensino FOREIGN KEY(mdeid)
  REFERENCES monitora.pi_modalidadeensino(mdeid) MATCH SIMPLE
  ON UPDATE CASCADE ON DELETE RESTRICT;

-- DROP TABLE IF EXISTS monitora.pi_objetivoppa_metappa;
CREATE TABLE monitora.pi_objetivo_ppa
(
  oppid SERIAL PRIMARY KEY,
  prsano CHAR(4),
  oppdesc VARCHAR(1000),
  oppnome VARCHAR(200),
  oppcod VARCHAR(4),
  oppstatus char default 'A'::character varying,
  CONSTRAINT fk_objppa_reference_programa FOREIGN KEY (prsano)
  REFERENCES monitora.programacaoexercicio (prsano) MATCH SIMPLE
  ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_oppstatus_objppa CHECK (oppstatus= ANY (ARRAY['A'::bpchar, 'I'::bpchar]))
);
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_objetivo_ppa TO usr_simec;

-- DROP TABLE IF EXISTS monitora.pi_metas_ppa;
CREATE TABLE monitora.pi_metas_ppa
(
  mppid SERIAL PRIMARY KEY,
  mppdesc VARCHAR(1000),
  mppcod CHARACTER(4),
  mppnome CHARACTER VARYING(400),
  mppstatus char default 'A'::character varying,
  prsano CHAR(4),
  CONSTRAINT fk_metppa_reference_programa FOREIGN KEY (prsano)
  REFERENCES monitora.programacaoexercicio (prsano) MATCH SIMPLE
  ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_mppstatus_metppa CHECK (mppstatus = ANY (ARRAY['A'::bpchar, 'I'::bpchar]))
);
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_metas_ppa TO usr_simec;

-- DROP TABLE IF EXISTS monitora.pi_objetivoppa_metappa;
CREATE TABLE monitora.pi_objetivoppa_metappa
(
  opmid serial NOT NULL,
  oppid integer NOT NULL,
  mppid integer NOT NULL,
  mpodata timestamp without time zone NOT NULL DEFAULT now(),
  CONSTRAINT pk_pi_objetivoppa_metappa PRIMARY KEY (opmid),
  CONSTRAINT fk_objetivoppa_metappa_reference_pi_objetivo_ppa FOREIGN KEY (oppid)
      REFERENCES monitora.pi_objetivo_ppa (oppid) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_objetivoppa_metappa_reference_metas_ppa FOREIGN KEY (mppid)
      REFERENCES monitora.pi_metas_ppa (mppid) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT
)
WITH (
  OIDS=FALSE
);
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_objetivoppa_metappa TO usr_simec;

-- DROP TABLE IF EXISTS monitora.pi_iniciativa_ppa; -- Renomeada pra pi_indicador_ppa
CREATE TABLE monitora.pi_iniciativa_ppa
(
  ippid serial NOT NULL,
  oppid integer NOT NULL,
  ippdesc VARCHAR(1000),
  ippnome CHARACTER VARYING(500),
  ippcod VARCHAR(4),
  ippstatus char default 'A'::character varying,
  prsano CHAR(4),
  PRIMARY KEY (ippid,oppid),
  CONSTRAINT fk_inippa_reference_obj FOREIGN KEY (oppid)
  REFERENCES monitora.pi_objetivo_ppa (oppid) MATCH SIMPLE
  ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_indppa_reference_programa FOREIGN KEY (prsano)
  REFERENCES monitora.programacaoexercicio (prsano) MATCH SIMPLE
  ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_ippstatus_indppa CHECK (ippstatus = ANY (ARRAY['A'::bpchar, 'I'::bpchar]))
);
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_iniciativa_ppa TO usr_simec;

-- DROP TABLE IF EXISTS monitora.pi_meta_pnc;
CREATE TABLE monitora.pi_meta_pnc
(
  mpnid SERIAL PRIMARY KEY,
  mpndesc VARCHAR(1000),
  mpnstatus char default 'A'::character varying,
  mpncod character(4) NOT NULL,
  mpnnome character varying(400) NOT NULL,
  prsano CHAR(4),
  CONSTRAINT fk_metppa_reference_programa FOREIGN KEY (prsano)
  REFERENCES monitora.programacaoexercicio (prsano) MATCH SIMPLE
  ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_mpnstatus_metpnc CHECK (mpnstatus = ANY (ARRAY['A'::bpchar, 'I'::bpchar]))
);
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_meta_pnc TO usr_simec;

-- DROP TABLE monitora.pi_indicador_pnc;
CREATE TABLE monitora.pi_indicador_pnc
(
  ipnid serial NOT NULL,
  mpnid integer NOT NULL,
  ipndesc character varying(1000),
  ipnstatus character(1) DEFAULT 'A'::character varying,
  prsano character(4),
  ipncod character(3),
  CONSTRAINT pi_indicador_pnc_pkey PRIMARY KEY (ipnid, mpnid),
  CONSTRAINT fk_indpnc_reference_meta FOREIGN KEY (mpnid)
      REFERENCES monitora.pi_meta_pnc (mpnid) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_indppa_reference_programa FOREIGN KEY (prsano)
      REFERENCES monitora.programacaoexercicio (prsano) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_ipnstatus_indpnc CHECK (ipnstatus = ANY (ARRAY['A'::bpchar, 'I'::bpchar]))
)
WITH (
  OIDS=FALSE
);
ALTER TABLE monitora.pi_indicador_pnc
  OWNER TO postgres;
GRANT ALL ON TABLE monitora.pi_indicador_pnc TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_indicador_pnc TO usr_simec;

CREATE TABLE monitora.pi_produto
(
  pprid serial NOT NULL,
  prsano character(4) NOT NULL,
  pprnome character varying(200) NOT NULL,
  pprdescricao character varying(500),
  pprstatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  CONSTRAINT pk_pi_produto PRIMARY KEY(pprid),
  CONSTRAINT ckc_pprstatus_pi_produto CHECK(pprstatus = ANY (ARRAY['I'::bpchar, 'A'::bpchar]))
)
WITH (
  OIDS=FALSE
);
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_produto TO usr_simec;

-- DROP TABLE monitora.pi_unidade_medida;
CREATE TABLE monitora.pi_unidade_medida
(
  pumid serial NOT NULL,
  prsano character(4) NOT NULL,
  pumnome character varying(100) NOT NULL,
  pumdescricao character varying(200),
  pumstatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  CONSTRAINT pk_pi_unidade_medida PRIMARY KEY(pumid),
  CONSTRAINT ckc_pi_unidade_medida CHECK(pumstatus = ANY (ARRAY['I'::bpchar, 'A'::bpchar]))
)
WITH (
  OIDS=FALSE
);
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_unidade_medida TO usr_simec;


GRANT SELECT, UPDATE ON TABLE planacomorc.pi_sniic_pisid_seq TO usr_simec;
GRANT SELECT, UPDATE ON TABLE planacomorc.pi_localizacao_pilid_seq TO usr_simec;

GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planacomorc.pi_cronograma TO usr_simec;



-------------------- 24/05/2017 --------------------

-- Table: planacomorc.cronograma

-- DROP TABLE planacomorc.cronograma;

CREATE TABLE planacomorc.cronograma
(
  croid serial NOT NULL, -- Chave Primária
  crodsc character varying(200) NOT NULL, -- Descrição
  CONSTRAINT pk_cronograma_croid PRIMARY KEY (croid)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planacomorc.cronograma
  OWNER TO postgres;
GRANT ALL ON TABLE planacomorc.cronograma TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planacomorc.cronograma TO usr_simec;
COMMENT ON TABLE planacomorc.cronograma
  IS 'Tabela de Cronogramas';
COMMENT ON COLUMN planacomorc.cronograma.croid IS 'Chave Primária';
COMMENT ON COLUMN planacomorc.cronograma.crodsc IS 'Descrição';


-- Table: planacomorc.cronograma_valor

-- DROP TABLE planacomorc.cronograma_valor;

CREATE TABLE planacomorc.cronograma_valor
(
  crvid serial NOT NULL, -- Chave Primária
  crvdsc character varying(200) NOT NULL, -- Descrição
  croid integer NOT NULL, -- Identificador do Cronograma
  CONSTRAINT pk_cronograma_valor_crvid PRIMARY KEY (crvid),
  CONSTRAINT fk_cronograma_valor_croid FOREIGN KEY (croid)
      REFERENCES planacomorc.cronograma (croid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planacomorc.cronograma_valor
  OWNER TO postgres;
GRANT ALL ON TABLE planacomorc.cronograma_valor TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planacomorc.cronograma_valor TO usr_simec;
COMMENT ON TABLE planacomorc.cronograma_valor
  IS 'Valores do Cronograma';
COMMENT ON COLUMN planacomorc.cronograma_valor.crvid IS 'Chave Primária';
COMMENT ON COLUMN planacomorc.cronograma_valor.crvdsc IS 'Descrição';
COMMENT ON COLUMN planacomorc.cronograma_valor.croid IS 'Identificador do Cronograma';




-- Table: planacomorc.pi_convenio

-- DROP TABLE planacomorc.pi_convenio;

CREATE TABLE planacomorc.pi_convenio
(
  pcoid serial NOT NULL, -- Chave Primária
  pliid integer NOT NULL, -- Plano Interno - PI
  pcoconvenio integer NOT NULL, -- Número do Convênio
  CONSTRAINT pk_pi_convenio_pcoid PRIMARY KEY (pcoid),
  CONSTRAINT fk_pi_complemento_pliid FOREIGN KEY (pliid)
      REFERENCES monitora.pi_planointerno (pliid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planacomorc.pi_convenio
  OWNER TO postgres;
GRANT ALL ON TABLE planacomorc.pi_convenio TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planacomorc.pi_convenio TO usr_simec;
COMMENT ON TABLE planacomorc.pi_convenio
  IS 'Vínculo do Plano Interno com Convênio';
COMMENT ON COLUMN planacomorc.pi_convenio.pcoid IS 'Chave Primária';
COMMENT ON COLUMN planacomorc.pi_convenio.pliid IS 'Plano Interno - PI';
COMMENT ON COLUMN planacomorc.pi_convenio.pcoconvenio IS 'Número do Convênio';


-- Table: planacomorc.pi_cronograma

-- DROP TABLE planacomorc.pi_cronograma;

CREATE TABLE planacomorc.pi_cronograma
(
  pcrid serial NOT NULL, -- Chave Primária
  pliid integer NOT NULL, -- Plano Interno - PI
  mescod character(2) NOT NULL, -- Mês
  pcrvalor double precision, -- Valor do Cronograma
  crvid integer NOT NULL -- Identificador do Cronograma Valor
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planacomorc.pi_cronograma
  OWNER TO postgres;
GRANT ALL ON TABLE planacomorc.pi_cronograma TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planacomorc.pi_cronograma TO usr_simec;
COMMENT ON TABLE planacomorc.pi_cronograma
  IS 'Cronograma Físico/Financeiro do Plano Interno';
COMMENT ON COLUMN planacomorc.pi_cronograma.pcrid IS 'Chave Primária';
COMMENT ON COLUMN planacomorc.pi_cronograma.pliid IS 'Plano Interno - PI';
COMMENT ON COLUMN planacomorc.pi_cronograma.mescod IS 'Mês';
COMMENT ON COLUMN planacomorc.pi_cronograma.pcrvalor IS 'Valor do Cronograma';
COMMENT ON COLUMN planacomorc.pi_cronograma.crvid IS 'Identificador do Cronograma Valor';

-- Table: planacomorc.pi_localizacao

-- DROP TABLE planacomorc.pi_localizacao;

CREATE TABLE planacomorc.pi_localizacao
(
  pilid serial NOT NULL, -- Chave Primária
  pliid integer NOT NULL, -- Plano Interno - PI
  estuf character(2), -- UF
  muncod character(7), -- Código IBGE do Município
  paiid integer, -- País
  CONSTRAINT pk_pi_localizacao_pilid PRIMARY KEY (pilid),
  CONSTRAINT fk_pi_complemento_pliid FOREIGN KEY (pliid)
      REFERENCES monitora.pi_planointerno (pliid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planacomorc.pi_localizacao
  OWNER TO postgres;
GRANT ALL ON TABLE planacomorc.pi_localizacao TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planacomorc.pi_localizacao TO usr_simec;
COMMENT ON TABLE planacomorc.pi_localizacao
  IS 'Vínculo do Plano Interno com Localização do Projeto';
COMMENT ON COLUMN planacomorc.pi_localizacao.pilid IS 'Chave Primária';
COMMENT ON COLUMN planacomorc.pi_localizacao.pliid IS 'Plano Interno - PI';
COMMENT ON COLUMN planacomorc.pi_localizacao.estuf IS 'UF';
COMMENT ON COLUMN planacomorc.pi_localizacao.muncod IS 'Código IBGE do Município';
COMMENT ON COLUMN planacomorc.pi_localizacao.paiid IS 'País';


-- Table: planacomorc.pi_responsavel

-- DROP TABLE planacomorc.pi_responsavel;

CREATE TABLE planacomorc.pi_responsavel
(
  pirid serial NOT NULL, -- Chave Primária
  pliid integer NOT NULL, -- Plano Interno - PI
  usucpf character(11) NOT NULL, -- CPF do usuário Responsável
  CONSTRAINT pk_pi_responsavel_pirid PRIMARY KEY (pirid),
  CONSTRAINT fk_pi_complemento_pliid FOREIGN KEY (pliid)
      REFERENCES monitora.pi_planointerno (pliid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT fk_pi_complemento_usucpf FOREIGN KEY (usucpf)
      REFERENCES seguranca.usuario (usucpf) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planacomorc.pi_responsavel
  OWNER TO postgres;
GRANT ALL ON TABLE planacomorc.pi_responsavel TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planacomorc.pi_responsavel TO usr_simec;
COMMENT ON TABLE planacomorc.pi_responsavel
  IS 'Vínculo do Plano Interno com Usuário';
COMMENT ON COLUMN planacomorc.pi_responsavel.pirid IS 'Chave Primária';
COMMENT ON COLUMN planacomorc.pi_responsavel.pliid IS 'Plano Interno - PI';
COMMENT ON COLUMN planacomorc.pi_responsavel.usucpf IS 'CPF do usuário Responsável';

-- Table: planacomorc.pi_sniic

-- DROP TABLE planacomorc.pi_sniic;

CREATE TABLE planacomorc.pi_sniic
(
  pisid serial NOT NULL, -- Chave Primária
  pliid integer NOT NULL, -- Plano Interno - PI
  pissniic integer NOT NULL, -- Número único do produto principal do sistema Mapas Culturais
  CONSTRAINT pk_pi_sniic_pisid PRIMARY KEY (pisid),
  CONSTRAINT fk_pi_complemento_pliid FOREIGN KEY (pliid)
      REFERENCES monitora.pi_planointerno (pliid) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE planacomorc.pi_sniic
  OWNER TO postgres;
GRANT ALL ON TABLE planacomorc.pi_sniic TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE planacomorc.pi_sniic TO usr_simec;
COMMENT ON TABLE planacomorc.pi_sniic
  IS 'Vínculo do Plano Interno com SNIIC';
COMMENT ON COLUMN planacomorc.pi_sniic.pisid IS 'Chave Primária';
COMMENT ON COLUMN planacomorc.pi_sniic.pliid IS 'Plano Interno - PI';
COMMENT ON COLUMN planacomorc.pi_sniic.pissniic IS 'Número único do produto principal do sistema Mapas Culturais';
