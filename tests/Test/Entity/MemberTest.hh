<?hh // strict

namespace Ivyhjk\Xml\Test\Entity;

use DOMDocument;
use SimpleXMLElement;
use Ivyhjk\Xml\Entity\Value;
use Ivyhjk\Xml\Entity\Member;
use Ivyhjk\Xml\Exception\InvalidNodeException;

/**
 * Member entity <member> tag test.
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Xml\Test\Entity
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
/* HH_FIXME[4123] */ /* HH_FIXME[2049] */
class MemberTest extends \PHPUnit_Framework_TestCase
{
    /**
     * Get an xml for tests.
     *
     * @param string $name The member name.
     * @param Vector<mixed> $parameters The parameters to send.
     * @param DOMDocument $document The root node.
     *
     * @return string
     */
    private function getXml(string $name, Value $value, DOMDocument $document) : string
    {
        $member = new Member($name, $value, $document);

        $element = $member->getElement();

        $document->appendChild($element);

        $xml = $document->saveXML();

        return preg_replace('/\n/', '', $xml);
    }

    /**
     * Test the getElement method.
     *
     * @return void
     */
    public function testGetElement() : void
    {
        $document = new DOMDocument();

        $value = new Value(Vector{
            'foo'
        }, $document);

        $xml = $this->getXml('hello', $value, $document);

        $expectedXml = '<?xml version="1.0"?><member><name>hello</name><value><string>foo</string></value></member>';

        static::assertEquals($expectedXml, $xml);
    }

    /**
     * Test the fromNode error.
     *
     * @return void
     */
    public function testFromNodeError() : void
    {
        $node = new SimpleXMLElement('<invalidTag/>');

        $this->expectException(InvalidNodeException::class);
        Member::fromNode($node, new DOMDocument());
    }

    /**
     * Test the fromNode method error, when missing <name> tag.
     *
     * @return void
     */
    public function testFromNodeInvalidName() : void
    {
        $node = new SimpleXMLElement('
            <member>
                <value>
                    <string>bar</string>
                </value>
            </member>
        ');

        $this->expectException(InvalidNodeException::class);
        Member::fromNode($node, new DOMDocument());
    }

    /**
     * Test the fromNode method error, when missing <value> tags.
     *
     * @return void
     */
    public function testFromNodeInvalidValue() : void
    {
        $node = new SimpleXMLElement('
            <member>
                <name>foo</name>
            </member>
        ');

        $this->expectException(InvalidNodeException::class);
        Member::fromNode($node, new DOMDocument());
    }

    /**
     * Test the fromNode method correct workflow.
     *
     * @return void
     */
    public function testFromNode() : void
    {
        $node = new SimpleXMLElement('
            <member>
                <name>foo</name>
                <value>
                    <string>bar</string>
                </value>
            </member>
        ');

        $memberEntity = Member::fromNode($node, new DOMDocument());

        $expectedValues = Vector{
            'bar'
        };

        static::assertSame('foo', $memberEntity->getName());
        static::assertEquals(
            $expectedValues,
            $memberEntity->getValue()->getValues()
        );
    }
}
