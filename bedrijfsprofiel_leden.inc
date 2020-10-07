<?php

class BedrijfsProfielLeden {
  public function __construct() {
    // initialize civicrm
    civicrm_initialize();
  }

  public function getPage() {
    $html = '
      <table>
        <tr>
          <th>Organisatie</th>
          <th>Lid sinds</th>
        </tr>
    ';

    $dao = $this->getMembers();
    while ($dao->fetch()) {

      $html .= '<tr>';
      $html .= '<td><a href="bedrijfsprofiel_leden_details?id=' . $dao->id . '">' . $dao->organization_name . '</a></td>';
      $html .= '<td>' . $dao->start_date . '</td>';
      $html .= '</tr>';
    }

    $html .= '</table>';

    return $html;
  }

  private function getMembers() {
    $sql = "
      select
        c.id,
        c.organization_name,
        m.start_date
      from
        civicrm_membership m
      inner join
        civicrm_contact c on c.id = m.contact_id
      where
        c.is_deleted = 0
      and
        m.owner_membership_id is null
      and
        m.status_id in (1, 2)
      order by
        c.sort_name
    ";

    $dao = CRM_Core_DAO::executeQuery($sql);

    return $dao;
  }
}
