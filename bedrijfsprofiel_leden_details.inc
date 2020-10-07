<?php

class BedrijfsProfielLedenDetails {
  private $contactID = 0;

  public function __construct($contactID) {
    $this->contactID = $contactID;

    // initialize civicrm
    civicrm_initialize();
  }

  public function getPage() {
    $html ='';

    $html .= $this->formatSectionTitle('Algemene gegevens');
    $html .= $this->AlgemeneGegevens();

    $html .= $this->formatSectionTitle('Bedrijfsomschrijving');
    $html .= $this->Bedrijfsomschrijving();

    $html .= $this->formatSectionTitle('Contactgegevens primaire contactpersoon');
    $html .= $this->formatSectionInfo("De primaire contactpersoon zal alle communicatie rond events & workshops automatisch ontvangen. Ook algemene vragen zoals logo’s, vragen omtrent de facturatie zullen aan deze contactpersoon gericht worden.");
    $html .= $this->ContactgegevensPrimaireContactpersoon();

    $html .= $this->formatSectionTitle('Contactgegevens CEO');
    $html .= $this->formatSectionInfo("De CEO wordt uitgenodigd voor netwerkevents en voor bijeenkomsten op strategisch niveau, zoals de algemene vergadering (vanaf standaard lidmaatschap).");
    $html .= $this->ContactgegevensCEO();

    $html .= $this->formatSectionTitle('Netwerkevenementen');
    $html .= $this->formatSectionInfo('Onderstaande personen zullen, naast de primaire contactpersoon en de CEO, uitgenodigd voor netwerkevents');
    $html .= $this->Netwerkevenementen();

    $html .= $this->formatSectionTitle('Workshops');
    $html .= $this->formatSectionInfo('Onderstaande personen worden, naast de primaire contactpersoon, uitgenodigd voor (thematische) workshops.');
    $html .= $this->Workshops();

    $html .= $this->formatSectionTitle('Internationaal');
    $html .= $this->Internationaal();

    $html .= $this->formatSectionTitle('Interessedomein(en)');
    $html .= $this->Interessedomeinen();

    $html .= $this->formatSectionTitle('Rol in de waardeketen');
    $html .= $this->formatSectionInfo('IK HEB ALLE TAGS GENOMEN ONDER "ROL". UITLEG ZOU JE IN BESCHRIJVING VAN DE TAG KUNNEN STOPPEN.');
    $html .= $this->Rollen();

    $html .= $this->formatSectionTitle('Deelname aan evenementen van de Blauwe Cluster');
    $html .= $this->formatSectionInfo('MOEILIJK OM EX-MEDEWERKERS TE TONEN: KAN ENKEL ALS WE ALTIJD EEN START EN EINDDATUM HEBBEN IN DE MEDEWERKER-VAN RELATIE.<br>IK TOON HIER ENKEL HUIDIGE MEDEWERKERS. HOPELIJK IS DAT VOLDOENDE.');
    $html .= $this->Deelnames();

    $html .= $this->formatSectionTitle('Telefoongesprekken/meetings');
    $html .= $this->formatSectionInfo('KAN ENKEL GETOOND WORDEN ALS HET BEDRIJF OOK INGEVULD IS IN HET VELD "MET CONTACT" VAN DE ACTIVITEIT');
    $html .= $this->Telefoongesprekken();

    $html .= $this->formatSectionTitle('Betrokkenheid in samenwerkingsinitiatieven');
    $html .= $this->Samenwerkingen();

    $html .= $this->formatSectionTitle('Privacy');
    $html .= $this->formatSectionInfo('De Blauwe Cluster vzw conformeert aan de nieuwe Algemene Verordening Gegevensbescherming. Je hebt een wettelijk recht om toegang tot, correctie en verwijdering van je persoonlijke gegevens te vragen. Je hebt ook het recht om de verwerking van je persoonlijke informatie te beperken.');

    $html .= '[&nbsp;&nbsp;] Ik ga akkoord dat DBC de gegevens van dit formulier bijhoudt in de eigen database.<br>';
    $html .= '<br><br>[Plaats]<br><br>';
    $html .= '<br><br>[Datum]<br><br>';
    $html .= '<br><br>[Handtekening]<br><br>';

    return $html;
  }

  private function formatSectionTitle($t) {
    return "<h2>$t</h2>";
  }

  private function formatSectionInfo($t) {
    return "<p class=\"sectionInfo\">$t</p>";
  }

  private function AlgemeneGegevens() {
    $html = '';

    $sql = "
      select
        c.organization_name,
        a.street_address,
        a.supplemental_address_1,
        a.postal_code,
        a.city,
        p.phone,
        w.url website,
        e.email,
        'GEEN VELD IN CIVI?' aantal_werknemers,
        'GEEN VELD IN CIVI?' jaarlijkse_omzet,
        o.percentage_omzet_blauw__31 percentage_omzet_blauw,
        grootte.label grootte,
        b.btw_nummer_1 btw_nummer,
        pubpriv.label publiek_prive,
        'GEEN VELD IN CIVI?' nacebel,
        mt.name type_lidmaatschap,
        m.source herkomst_lidmaatschap
      from
        civicrm_contact c
      inner join
        civicrm_membership m on m.contact_id = c.id
      inner join
        civicrm_membership_type mt on mt.id = m.membership_type_id
      left outer join
        civicrm_address a on a.contact_id = c.id and a.is_primary = 1
      left outer join
        civicrm_phone p on p.contact_id = c.id and p.is_primary = 1
      left outer join
        civicrm_email e on e.contact_id = c.id and e.is_primary = 1
      left outer join
        civicrm_website w on w.contact_id = c.id and w.website_type_id = 1
      left outer join
        civicrm_value_organisatie_i_5 o on o.entity_id = c.id
      left outer join
        civicrm_option_value grootte on grootte.value = o.grootte_27 and grootte.option_group_id = 100
      left outer join
        civicrm_value_boekhouding_1  b on b.entity_id  = c.id
      left outer join
        civicrm_option_value pubpriv on pubpriv.value = o.publiek_of_privaat__50 and pubpriv.option_group_id = 109
      where
        c.id = %1
      and
        c.is_deleted = 0
      and
        c.contact_type = 'Organization'
    ";
    $sqlParams = [
      1 => [$this->contactID, 'Integer'],
    ];
    $dao = CRM_Core_DAO::executeQuery($sql, $sqlParams);
    $dao->fetch();

    $html = '<table>';
    $html .= "<tr><td>Bedrijfsnaam</td><td>{$dao->organization_name}</td></tr>";
    $html .= "<tr><td>Adres (straat, nr, postcode, stad)</td><td>{$dao->street_address}<br>";
    $html .= $dao->supplemental_address_1 ? "{$dao->supplemental_address_1}<br>" : '';
    $html .= "{$dao->postal_code} {$dao->city}</td></tr>";
    $html .= "<tr><td>Algemeen Telefoonnummer</td><td>{$dao->phone}</td></tr>";
    $html .= "<tr><td>Algemeen E-mailadres</td><td>{$dao->email}</td></tr>";
    $html .= "<tr><td>Website</td><td>{$dao->website}</td></tr>";
    $html .= "<tr><td>Aantal werknemers</td><td>{$dao->aantal_werknemers}</td></tr>";
    $html .= "<tr><td>Jaarlijkse omzet</td><td>{$dao->jaarlijkse_omzet}</td></tr>";
    $html .= "<tr><td>Percentage omzet 'blauw'</td><td>{$dao->percentage_omzet_blauw}</td></tr>";
    $html .= "<tr><td>Balanstotaal</td><td>{$dao->website}</td></tr>";
    $html .= "<tr><td>Grootte (GO, MO, KO)</td><td>{$dao->grootte}</td></tr>";
    $html .= "<tr><td>BTW nummer</td><td>{$dao->btw_nummer}</td></tr>";
    $html .= "<tr><td>Publiek of privaat</td><td>{$dao->publiek_prive}</td></tr>";
    $html .= "<tr><td>NACEBEL code (hoofdactiviteit)</td><td>{$dao->nacebel}</td></tr>";
    $html .= "<tr><td>Type lidmaatschap</td><td>{$dao->type_lidmaatschap}</td></tr>";
    $html .= "<tr><td>Herkomst lidmaatschap</td><td>{$dao->herkomst_lidmaatschap}</td></tr>";
    $html .= '</table>';

    return $html;
  }

  private function Bedrijfsomschrijving() {
    $html = '';

    $sql = "
      select
        o.activiteiten_nl_25,
        o.activiteiten_en_26
      from
        civicrm_contact c
      left outer join
        civicrm_value_organisatie_i_5 o on o.entity_id = c.id
      where
        c.id = %1
      and
        c.is_deleted = 0
      and
        c.contact_type = 'Organization'
    ";
    $sqlParams = [
      1 => [$this->contactID, 'Integer'],
    ];
    $dao = CRM_Core_DAO::executeQuery($sql, $sqlParams);
    $dao->fetch();

    $html = '<table>';
    $html .= "<tr><td>Nederlands (1 slagzin) – wordt voor de ledenpagina op de website gebruikt</td><td>{$dao->activiteiten_nl_25}</td></tr>";
    $html .= "<tr><td>Engels (1 slagzin)</td><td>{$dao->activiteiten_en_26}</td></tr>";
    $html .= '</table>';

    return $html;
  }

  private function ContactgegevensPrimaireContactpersoon() {
    $html = '';

    $sql = "
      select
        concat(c.first_name, ' ', c.last_name) naam
        , c.job_title
        , e.email
        , tel_work.phone work_phone
        , tel_mobile.phone mobile_phone
      from
        civicrm_contact c
      inner join
        civicrm_entity_tag et on et.entity_id = c.id and et.entity_table = 'civicrm_contact' and et.tag_id = (select t.id from civicrm_tag t where t.name = 'Primary contact')
      left outer join
        civicrm_email e on e.contact_id = c.id and e.is_primary = 1
      left outer join
        civicrm_phone tel_work on tel_work.contact_id = c.id and tel_work.location_type_id = 2 and tel_work.phone_type_id = 1
      left outer join
        civicrm_phone tel_mobile on tel_mobile.contact_id = c.id and tel_mobile.phone_type_id = 2
      where
        c.employer_id = %1
      and
        c.is_deleted = 0
      and
        c.is_deceased = 0
    ";
    $sqlParams = [
      1 => [$this->contactID, 'Integer'],
    ];
    $dao = CRM_Core_DAO::executeQuery($sql, $sqlParams);
    $dao->fetch();

    $html = '<table>';
    $html .= "<tr><td>Voornaam en naam</td><td>{$dao->naam}</td></tr>";
    $html .= "<tr><td>Functie</td><td>{$dao->job_title}</td></tr>";
    $html .= "<tr><td>Telefoonnummer</td><td>{$dao->work_phone}</td></tr>";
    $html .= "<tr><td>Gsm-nummer</td><td>{$dao->mobile_phone}</td></tr>";
    $html .= "<tr><td>E-mailadres</td><td>{$dao->email}</td></tr>";
    $html .= '</table>';

    return $html;
  }

  private function ContactgegevensCEO() {
    $html = '';

    $html = '<p>HOE WEET IK WIE CEO IS? IS ER EEN KENMERK?</p>';
    return $html;
  }

  private function Netwerkevenementen() {
    $html = '';

    $sql = "
      select
        c.first_name
        , c.last_name
        , e.email
      from
        civicrm_contact c
      inner join
        civicrm_entity_tag et on et.entity_id = c.id and et.entity_table = 'civicrm_contact' and et.tag_id = (select t.id from civicrm_tag t where t.name = 'netwerkevents')
      left outer join
        civicrm_email e on e.contact_id = c.id and e.is_primary = 1
      where
        c.employer_id = %1
      and
        c.is_deleted = 0
      and
        c.is_deceased = 0
      order by
        c.sort_name
    ";
    $sqlParams = [
      1 => [$this->contactID, 'Integer'],
    ];
    $dao = CRM_Core_DAO::executeQuery($sql, $sqlParams);

    $html = '<table>';
    $html .= "<tr><th>Voornaam</th><th>Naam</th><th>E-mailadres</th></tr>";
    while ($dao->fetch()) {
      $html .= "<tr><td>{$dao->first_name}</td>";
      $html .= "<td>{$dao->last_name}</td>";
      $html .= "<td>{$dao->email}</td></tr>";
    }
    $html .= '</table>';

    return $html;
  }

  private function Workshops() {
    $html = '';

    $sql = "
      select
        c.first_name
        , c.last_name
        , e.email
      from
        civicrm_contact c
      inner join
        civicrm_entity_tag et on et.entity_id = c.id and et.entity_table = 'civicrm_contact' and et.tag_id = (select t.id from civicrm_tag t where t.name = 'workshops')
      left outer join
        civicrm_email e on e.contact_id = c.id and e.is_primary = 1
      where
        c.employer_id = %1
      and
        c.is_deleted = 0
      and
        c.is_deceased = 0
      order by
        c.sort_name
    ";
    $sqlParams = [
      1 => [$this->contactID, 'Integer'],
    ];
    $dao = CRM_Core_DAO::executeQuery($sql, $sqlParams);

    $html = '<table>';
    $html .= "<tr><th>Voornaam</th><th>Naam</th><th>E-mailadres</th></tr>";
    while ($dao->fetch()) {
      $html .= "<tr><td>{$dao->first_name}</td>";
      $html .= "<td>{$dao->last_name}</td>";
      $html .= "<td>{$dao->email}</td></tr>";
    }
    $html .= '</table>';

    return $html;
  }

  private function Internationaal() {
    $html = '';

    $sql = "
      select
        filiaal.name filiaal
        , if(o.export_internationaal_actief__30=1,'Ja',if(o.export_internationaal_actief__30=0,'Nee','')) internationaal
        , 'GEEN VELD IN CIVI?' export_landen
      from
        civicrm_contact c
      left outer join
        civicrm_value_organisatie_i_5 o on o.entity_id = c.id
      left outer join
        civicrm_option_value filiaal on filiaal.value = o.hoofdkantoor_of_filiaal_28 and filiaal.option_group_id = 101
      where
        c.id = %1
      and
        c.is_deleted = 0
      and
        c.contact_type = 'Organization'
    ";
    $sqlParams = [
      1 => [$this->contactID, 'Integer'],
    ];
    $dao = CRM_Core_DAO::executeQuery($sql, $sqlParams);
    $dao->fetch();

    $html = '<table>';
    $html .= "<tr><td>Hoofdkantoor of filiaal</td><td>{$dao->filiaal}</td></tr>";
    $html .= "<tr><td>Internationaal actief?</td><td>{$dao->internationaal}</td></tr>";
    $html .= "<tr><td>Export (Landen)</td><td>{$dao->export_landen}</td></tr>";
    $html .= '</table>';

    return $html;
  }

  private function Interessedomeinen() {
    $html = '';

    $sql = "
      select
        t.name
        , (select if(et.id IS NOT NULL, 'x', '') from civicrm_entity_tag et where et.tag_id = t.id and et.entity_id = %1) has_tag
      from
        civicrm_tag t
      where
        parent_id = (select pt.id from civicrm_tag pt where name = 'Domein')
      order by
        t.name
    ";
    $sqlParams = [
      1 => [$this->contactID, 'Integer'],
    ];
    $dao = CRM_Core_DAO::executeQuery($sql, $sqlParams);

    $html = '<table>';
    while ($dao->fetch()) {
      $html .= "<tr><td>{$dao->name}</td>";
      $html .= "<td>{$dao->has_tag}</td></tr>";
    }
    $html .= '</table>';

    return $html;
  }

  private function Rollen() {
    $html = '';

    $sql = "
      select
        t.name
        , (select if(et.id IS NOT NULL, 'x', '') from civicrm_entity_tag et where et.tag_id = t.id and et.entity_id = %1) has_tag
        , t.description
      from
        civicrm_tag t
      where
        parent_id = (select pt.id from civicrm_tag pt where name = 'Rol')
      order by
        t.name
    ";
    $sqlParams = [
      1 => [$this->contactID, 'Integer'],
    ];
    $dao = CRM_Core_DAO::executeQuery($sql, $sqlParams);

    $html = '<table>';
    while ($dao->fetch()) {
      $html .= "<tr><td>{$dao->name}</td>";
      $html .= "<td>{$dao->has_tag}</td>";
      $html .= "<td>{$dao->description}</td></tr>";
    }
    $html .= '</table>';

    return $html;
  }

  private function Deelnames() {
    $html = '';

    $sql = "
      select
        e.title
        , e.start_date
        , concat(c.first_name, ' ', c.last_name) name
      from
        civicrm_contact c
      inner join
        civicrm_participant p on p.contact_id = c.id
      inner join
        civicrm_event e on p.event_id = e.id
      where
        c.employer_id = %1
      and
        c.is_deleted = 0
      order by
        e.start_date desc
        , c.sort_name
    ";
    $sqlParams = [
      1 => [$this->contactID, 'Integer'],
    ];
    $dao = CRM_Core_DAO::executeQuery($sql, $sqlParams);

    $html = '<table>';
    $html .= "<tr><th>Evenement</th><th>Evenementdatum</th><th>Naam deelnemer</th></tr>";
    while ($dao->fetch()) {
      $html .= "<tr><td>{$dao->title}</td>";
      $html .= "<td>{$dao->start_date}</td>";
      $html .= "<td>{$dao->name}</td></tr>";
    }
    $html .= '</table>';

    return $html;
  }

  private function Telefoongesprekken() {

  }

  private function Samenwerkingen() {
    $html = '';

    $sql = "
      select
        c.subject
        , ct.title
        , group_concat(org.organization_name separator ', ') partners
      from
        civicrm_case c
      inner join
        civicrm_case_type ct on c.case_type_id = ct.id
      inner join
        civicrm_case_contact cc on c.id = cc.case_id
      left outer join
        civicrm_relationship r on r.case_id = c.id and r.relationship_type_id = 19
      left outer join
        civicrm_contact org on org.id = r.contact_id_b
      where
        cc.contact_id = 6
      group by
        c.subject
        , ct.title
      order by
        org.organization_name
    ";
    $sqlParams = [
      1 => [$this->contactID, 'Integer'],
    ];
    $dao = CRM_Core_DAO::executeQuery($sql, $sqlParams);

    $html = '<table>';
    $html .= "<tr><th>Type samenwerking (dossiertype)</th><th>Naam dossier</th><th>Partners</th></tr>";
    while ($dao->fetch()) {
      $html .= "<tr><td>{$dao->title}</td>";
      $html .= "<td>{$dao->subject}</td>";
      $html .= "<td>{$dao->partners}</td></tr>";
    }
    $html .= '</table>';

    return $html;
  }

}
