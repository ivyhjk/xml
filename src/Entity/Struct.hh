<?hh // strict

namespace Ivyhjk\Xml\Entity;

use DOMElement;
use DOMDocument;
use SimpleXMLElement;
use Ivyhjk\Xml\Exception\InvalidNodeException;

/**
 * <methodName> tag concrete class.
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Xml\Entity
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
class Struct extends Tag
{
    /**
     * The XML tag name
     *
     * @var string
     */
    const string TAG_NAME = 'struct';

    /**
     * Generate a new <struct> tag instance.
     *
     * @param Vector<Ivyhjk\Xml\Entity\Member> $members
     * @param DOMDocument $document
     *
     * @return void
     */
    public function __construct(private Vector<Member> $members, DOMDocument $document) : void
    {
        parent::__construct($document);
    }

    /**
     * Get the associated <member> tags classes instance.
     *
     * @return Vector<Ivyhjk\Xml\Entity\Member>
     */
    public function getMembers() : Vector<Member>
    {
        return $this->members;
    }

    /**
     * Get the <methodCall> tag as a DOMelement.
     *
     * @return DOMElement
     */
    public function getElement() : DOMElement
    {
        $element = $this
            ->getDocument()
            ->createElement(static::TAG_NAME);

        foreach ($this->getMembers() as $member) {
            $element->appendChild($member->getElement());
        }

        return $element;
    }

    /**
     * Generate a new Struct from a given SimpleXMLElement $node.
     *
     * @param SimpleXMLElement $node
     * @param DOMDocument $document
     *
     * @return Ivyhjk\Xml\Entity\Struct
     * @throws Ivyhjk\Xml\Exception\InvalidNodeException
     */
    public static function fromNode(SimpleXMLElement $node, DOMDocument $document) : Struct
    {
        if ($node->getName() !== static::TAG_NAME) {
            throw new InvalidNodeException();
        }

        $memberNodes = $node->xpath(Member::TAG_NAME);

        $memberEntities = Vector{};

        foreach ($memberNodes as $memberNode) {
            $memberEntity = Member::fromNode($memberNode, $document);

            $memberEntities->add($memberEntity);
        }

        return new Struct($memberEntities, $document);
    }
}
